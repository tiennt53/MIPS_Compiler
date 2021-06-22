#include <bits/stdc++.h>

#define INIT_OFFSET 4194304

using namespace std;

vector<string> input;
map<string, int> reg;
map<string, int> opcode;
map<string, int> labelAddress;

void init() {
	opcode["add"] = 32;
	opcode["and"] = 36;
	opcode["sub"] = 34;
	opcode["nor"] = 39;
	opcode["or"] = 37;
	opcode["slt"] = 42;

	opcode["addi"] = (8 << 26);
	opcode["lw"] = (35 << 26);
	opcode["sw"] = (43 << 26);
	opcode["beq"] = (4 << 26);
	opcode["bne"] = (5 << 26);

	opcode["j"] = (2 << 26);

	reg["$zero"] = 0;
	reg["$at"] = 1;
	reg["$v0"] = 2;
	reg["$v1"] = 3;
	reg["$a0"] = 4;
	reg["$a1"] = 5;
	reg["$a2"] = 6;
	reg["$a3"] = 7;
	reg["$t0"] = 8;
	reg["$t1"] = 9;
	reg["$t2"] = 10;
	reg["$t3"] = 11;
	reg["$t4"] = 12;
	reg["$t5"] = 13;
	reg["$t6"] = 14;
	reg["$t7"] = 15;
	reg["$t8"] = 16;
	reg["$t9"] = 17;
	reg["$s0"] = 18;
	reg["$s1"] = 19;
	reg["$s2"] = 20;
	reg["$s3"] = 21;
	reg["$s4"] = 22;
	reg["$s5"] = 23;
	reg["$s6"] = 24;
	reg["$s7"] = 25;
	reg["$k0"] = 26;
	reg["$k1"] = 27;
	reg["$gp"] = 28;
	reg["$sp"] = 29;
	reg["$fp"] = 30;
	reg["$ra"] = 31;
}

void DeleteComment(vector<string> &input) {
    /// Arguments: lines of input
    /// Output: In a clear form of command
    vector<string> clearInput;

    for (int t = 0; t < input.size(); t++) {
        string s = input[t];
        if (s == "") continue;
        for (int i = 0; i < s.size() - 1; i++) {
            if (s[i] == '#') {
                s.erase(i, s.size() - i);
                break;
            }
            else if (s[i] != ' ' && s[i + 1] == ' ') {
                /// have a form: "abcxyz "
                s[i + 1] = '*';
            }
        }
        s.erase(remove(s.begin(), s.end(), ' '), s.end());
        replace(s.begin(), s.end(), '*', ' ');
        if (s != "")
            clearInput.push_back(s);
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
    /// Output: input with no label, also export label table into temp file. ("temp.txt")
}

int GenerateBinary(string cmd) {
    /// Arguments: a string of command
    /// Output: binary code
    return 0;
}

void FirstPass() {
    DeleteComment(input);
    map<string, int> labelAddress;
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
