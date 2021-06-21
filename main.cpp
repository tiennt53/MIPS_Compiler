#include <bits/stdc++.h>

using namespace std;

vector<string> input;

void DeleleComment(vector<string> &input) {
    /// Arguments: lines of input
    /// Output: In a clear form of command
}

void CalculateImmediate(vector<string> input, vector<int> &label) {
    /// Arguments: lines of input (clear form)
    /// Output: list of calculated label
}

void BuildLabelTable(vector<string> &input, vector<int> label) {
    /// Arguments: lines of input (clear form); label list
    /// Output: input with no label, also export label table into temp file. ("temp.txt")
}

int GenerateBinary(string cmd) {
    /// Arguments: a string of command
    /// Output: binary code
}

void FirstPass() {
    DeleleComment(&input);
    vector<int> calculatedLabel;
    CalculateImmediate(input, &calculatedLabel);
    BuildLabelTable(&input, calculatedLabel);
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
    while (cin >> line) {
        input.push_back(line);
    }
    FirstPass();
    SecondPass();
}

