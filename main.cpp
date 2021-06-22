#include <bits/stdc++.h>

#define INIT_OFFSET 4194304

using namespace std;

vector<string> input;
map<string, int> reg;
map<string, pair<int, int> > opcode;
map<string, int> labelAddress;

void init() {
    /// opcode: string: command name, pair<int, int>: binary value and command type: 0 = R, 1 = I, 2 = J
	/// R_TYPE
	opcode["add"]  = make_pair(32, 0);
	opcode["addu"]  = make_pair(33, 0);
    opcode["sub"]  = make_pair(34, 0);
	opcode["subu"]  = make_pair(35, 0);
	opcode["and"]  = make_pair(36, 0);
    opcode["or"]   = make_pair(37, 0);
	opcode["nor"]  = make_pair(39, 0);
	opcode["slt"]  = make_pair(42, 0);
	opcode["sltu"]  = make_pair(43, 0);
	opcode["sll"]  = make_pair(0, 0);
	opcode["srl"]  = make_pair(2, 0);
    /// I_TYPE
	opcode["addi"] = make_pair(8 << 26, 1);
	opcode["lw"]   = make_pair(35 << 26, 1);
	opcode["sw"]   = make_pair(43 << 26, 1);
	opcode["beq"]  = make_pair(4 << 26, 1);
	opcode["bne"]  = make_pair(5 << 26, 1);
    /// J_TYPE
	opcode["j"]    = make_pair(2 << 26, 2);
    opcode["jal"]  = make_pair(3 << 26, 2);
    string reg_list[32] = {"$zero", "$at", "$v0", "$v1",
                        "$a0", "$a1", "$a2", "$a3",
                        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                        "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
                        "$t8", "$t9",
                        "$k0", "$k1",
                        "$gp", "$sp", "$fp", "$ra"};

    for (int i = 0; i < 32; i++)
        reg[reg_list[i]] = i;
}

unsigned int Complement(int n) {
   int number_of_bits = floor(log2(n))+1;

   // XOR the given integer with poe(2,
   // number_of_bits-1 and print the result
   return ((1 << number_of_bits) - 1) ^ n;
}
string binaryConverter(unsigned int num) {
	unsigned int bin;
	string s = "";
	unsigned int i = 0;
	while (num > 0)
	{
		i++;
		bin = num % 2;
		if (bin) {
			s += "1";
		}
		else {
			s += "0";
		}
		num /= 2;
	}
	for (unsigned int j = 32-i; j > 0; j--) {
		s += "0";
	}
	reverse(s.begin(), s.end());

	return s;

}

struct Commands {
    /// Example call:
    /// Commands command;
    /// int value = command.R_Type(0, 1, 2, 3, 0, 0);
    unsigned int R_Type(string str) {
        regex type1("([a-z]+) ([$][a-z]*[0-9]|[$][a-z]*), ([$][a-z]*[0-9]|[$][a-z]*), ([$][a-z]+[0-9]|[$][a-z]+)");
        smatch matches;
        regex_search(str, matches, type1);
        unsigned int binNum = 0;
        if (!matches.empty()) {
            binNum = opcode[matches.str(1)].first;
            binNum += (reg[matches.str(2)] << 11);
            binNum += (reg[matches.str(3)] << 21);
            binNum += (reg[matches.str(4)] << 16);
        }
        return binNum;
    }
    unsigned int I_Type(string str) {
        /// addi $t1, $t2, 4; beq $t1, $t2, A; lw $t1, 4($t2)
        regex type1("([a-z]+) {1,10}([$][a-z]+[0-9]*|[$][a-z]+), {1,10}([$][a-z]+[0-9]*|[$][a-z]+), {1,10}(.+)?");
        regex type2("([a-z]+) {1,10}([$][a-z]+[0-9]*|[$][a-z]+), {1,10}([0-9]+) {0,10}[(]([$][a-z]+[0-9]*|[$][a-z]+)[)]");
        vector<smatch> matches(2);
        regex_search(str, matches[0], type1);
        regex_search(str, matches[1], type2);

        unsigned int binNum = 0;
        if (!matches[0].empty()) {
            binNum = opcode[matches[0].str(1)].first;
            binNum += (reg[matches[0].str(3)] << 21);
            binNum += (reg[matches[0].str(2)] << 16);
            if (labelAddress.find(matches[0].str(4)) != labelAddress.end()) {
                int imm = (labelAddress[matches[0].str(4)] - labelAddress[str] - 4) >> 2;
                imm %= 2 << 15;
                binNum += imm;
            }
            else {
                stringstream val(matches[0].str(4));
                unsigned int num = 0;
                val >> num;
                if (num < 0) num = Complement(num);
                num %= 2<<15;
                binNum += num;
            }
        }
        else if (!matches[1].empty()) {
            binNum = opcode[matches[1].str(1)].first;
            binNum += (reg[matches[1].str(4)] << 21);
            binNum += (reg[matches[1].str(2)] << 16);
            stringstream val(matches[1].str(3));
            unsigned int num = 0;
            val >> num;
            if (num < 0) num = Complement(num);
            num %= 2<<15;
            binNum += num;
        }
        return binNum;
    }
    int J_Type(string str) {
        regex type1("([j]{1}) {1,10}([a-zA-Z]{1}[a-zA-Z0-9$._]*)");
        smatch matches;
        unsigned int binNum = 0;
        regex_search(str, matches, type1);
        if (!matches.empty()) {
            binNum = opcode[matches.str(1)].first;
            int address = labelAddress[matches.str(2)] % (2<<28);
            address >>= 2;
            binNum += address;
        }
        return binNum;
    }
} c;

void DeleteComment(vector<string> &input) {
    /// Arguments: lines of input
    /// Output: In a clear form of command
    vector<string> clearInput;

    for (string line: input) {
        if (line == "") continue;
        for (int i = 0; i < line.size() - 1; i++) {
            if (line[i] == '#') {
                line.erase(i, line.size() - i);
                break;
            }
            else if (line[i] != ' ' && line[i + 1] == ' ') {
                /// have a form: "abcxyz "
                line[i + 1] = '*';
            }
        }
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        replace(line.begin(), line.end(), '*', ' ');
        if (line != "")
            clearInput.push_back(line);
    }
    input = clearInput;
}

void CalculateImmediate(vector<string> input, map<string, int> &label) {
    /// Arguments: lines of input (clear form)
    /// Output: list of calculated label
	/// input = { "j nxt1", "j nxt2", "nxt1:", "nxt2:", "add $t1, $zero, $zero", "j nxt3", "add $t0, $zero, $zero", "nxt3: add $t3, $zero, $zero" }; ///sample MIPS code
	int offset = INIT_OFFSET - 4; /// 0x00400000 - 4
	queue<string> labelQueue; /// contain continuosly labels (there aren't and instruction between them)
	for (string line: input) {
		size_t findLabel = line.find(":"); /// if there is a ':' in line, there is a label before it
        if (findLabel != string::npos) { /// if found label
			labelQueue.push(line.substr(0, findLabel)); /// findLabel is also size of label
		}
		if (findLabel == string::npos || findLabel + 1 < line.size()) { /// if label is not found or label is found but there is an instruction after that
 			offset += 4;                                                /// then all labels in queue have the same address at current offset
			while (!labelQueue.empty()) {
                string currentLabel = labelQueue.front();
                labelQueue.pop();
                label[currentLabel] = offset;
            }
		}
	}
}

void BuildLabelTable(vector<string> &input, map<string, int> label) {
    /// Arguments: lines of input (clear form); label list
    /// Output: input with no label, also export label table into temp file. ("labelAddress.txt")

    /// clear label in input
    vector<string> clearInput;
    for (string line: input) {
        if (line == "") continue;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == ':') {
                line.erase(line.begin(), line.begin() + i + 1);
                while (line[0] == ' ') {
                    line.erase(line.begin());
                }
                break;
            }
        }
        if (line != "")
            clearInput.push_back(line);
    }
    input = clearInput;
    /// export label table to file
    /// line format: <label name> <address value>
    fstream ofs;
	ofs.open("labelAddress.txt", ios::out | ios::trunc);
    for (auto lb: label) {
        ofs << lb.first << " " << lb.second << endl;
    }
    ofs.close();
}

int GenerateBinary(string cmd) {
    /// Arguments: a string of command
    /// Output: binary code
    string classifier = cmd.substr(0, cmd.find(' '));
    unsigned int binNum = 0;
    if (opcode[classifier].second == 0) {
        binNum = c.R_Type(cmd);
    }
    else if (opcode[classifier].second == 1) {
        binNum = c.I_Type(cmd);
    }
    else {
        binNum = c.J_Type(cmd);
    }
    return (binNum);
}

void FirstPass() {
    DeleteComment(input);
    CalculateImmediate(input, labelAddress);
    BuildLabelTable(input, labelAddress);
}

void SecondPass() {
    for (string line: input) {
        cout << line << endl;
	/// in ra cho dep
        cout << "> " << "0x" << setfill('0') << setw(8) << right << hex << GenerateBinary(line) << endl;
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    string line;
    while (getline(cin, line)) {
        input.push_back(line);
    }
    init();
    FirstPass();
    SecondPass();
}
