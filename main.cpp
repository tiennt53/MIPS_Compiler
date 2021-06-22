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
	opcode["and"]  = make_pair(36, 0);
	opcode["sub"]  = make_pair(34, 0);
	opcode["nor"]  = make_pair(39, 0);
	opcode["or"]   = make_pair(37, 0);
	opcode["slt"]  = make_pair(42, 0);
    /// I_TYPE
	opcode["addi"] = make_pair(8 << 26, 1);
	opcode["lw"]   = make_pair(35 << 26, 1);
	opcode["sw"]   = make_pair(43 << 26, 1);
	opcode["beq"]  = make_pair(4 << 26, 1);
	opcode["bne"]  = make_pair(5 << 26, 1);
    /// J_TYPE
	opcode["j"]    = make_pair(2 << 26, 2);

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

struct Commands {
    /// Example call:
    /// Commands command;
    /// int value = command.R_Type(0, 1, 2, 3, 0, 0);
    int R_Type(int opcode, int rs, int rt, int rd, int shamt, int funct) {
        /// return binary value
        return 1;
    }
    int I_Type(int opcode, int rs, int rt, int immediate) {
        /// return binary value
        return 1;
    }
    int J_Type(int opcode, int address) {
        /// return binary value
        return 1;
    }
};

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
                line.erase(line.begin(), line.begin() + i);
                if (line[0] == ' ')
                    line.erase(line.begin());
                break;
            }
        }
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
    string regex_pattern = "";
    return 0;
}

void FirstPass() {
    DeleteComment(input);
    CalculateImmediate(input, labelAddress);
    BuildLabelTable(input, labelAddress);
}

void SecondPass() {
    for (int i = 0; i < input.size(); i++) {
        int binary = GenerateBinary(input[i]);
        /// do something here
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
