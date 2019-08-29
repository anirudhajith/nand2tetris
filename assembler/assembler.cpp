#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

using namespace std;

int main(int argc, char** argv) {
    
    // make sure command is executed properly
    if (argc == 1) {
        cerr << "ERROR: Provide asm file as command line argument" << endl;
        return 1;
    } else if (argc > 2) {
        cerr << "ERROR: Provide only one asm file as command line argument" << endl;
        return 1;
    }

    string filename = argv[1];
    string asmFileContents;
    
    // read file contents into asmFileContents
    if (ifstream asmFile = ifstream(filename.c_str())) {
        asmFileContents.assign((istreambuf_iterator<char>(asmFile)), (istreambuf_iterator<char>()));
        asmFile.close();
    } else {
        cerr << "ERROR: File not found" << endl;
        return 1;
    }

    // remove all comments, whitespace, multi-newlines from asmFileContents
    regex anyComment("(\\/\\/.*|\\/\\*[\\s\\S]*?\\*\\/)");
    regex newline("[\\r\\n]+");
    regex whitespace("[     ]");

    asmFileContents = regex_replace(asmFileContents, anyComment, "");
    asmFileContents = regex_replace(asmFileContents, whitespace, "");
    asmFileContents = regex_replace(asmFileContents, newline, "\n");
    if (asmFileContents[0] == '\n' || asmFileContents[0] == '\r') asmFileContents.erase(asmFileContents.begin());
    if (asmFileContents[0] == '\n' || asmFileContents[0] == '\r') asmFileContents.erase(asmFileContents.begin());
    if (asmFileContents.back() == '\n' || asmFileContents.back() == '\r') asmFileContents.erase(asmFileContents.end() - 1);
    if (asmFileContents.back() == '\n' || asmFileContents.back() == '\r') asmFileContents.erase(asmFileContents.end() - 1);

    // save cleaned asmFileContents into assemberOut.ir
    ofstream outFile("assemblerOut.ir");
    outFile << asmFileContents;
    outFile.close();
    
    // populate symbolTable with standard symbols
    unordered_map<string, int> symbolTable;

    symbolTable["SP"] = 0;
    symbolTable["LCL"] = 1;
    symbolTable["ARG"] = 2;
    symbolTable["THIS"] = 3;
    symbolTable["THAT"] = 4;
    symbolTable["R0"] = 0;
    symbolTable["R1"] = 1;
    symbolTable["R2"] = 2;
    symbolTable["R3"] = 3;
    symbolTable["R4"] = 4;
    symbolTable["R5"] = 5;
    symbolTable["R6"] = 6;
    symbolTable["R7"] = 7;
    symbolTable["R8"] = 8;
    symbolTable["R9"] = 9;
    symbolTable["R10"] = 10;
    symbolTable["R11"] = 11;
    symbolTable["R12"] = 12;
    symbolTable["R13"] = 13;
    symbolTable["R14"] = 14;
    symbolTable["R15"] = 15;
    symbolTable["SCREEN"] = 16384;
    symbolTable["KBD"] = 24576;

    // insert all labels into symbolTable
    istringstream asmStream(asmFileContents);
    int variableCount = 0;
    int lineNumber = 0;

    for(string line; getline(asmStream, line); lineNumber++) {
        if (line[0] == '(') {
            string label = line.substr(1, line.size() - 2);
            symbolTable[label] = lineNumber;
        }
    }

    // insert all variables into symbolTable
    asmStream.clear();
    asmStream.seekg(0);

    for(string line; getline(asmStream, line); lineNumber++) {
        if (line[0] == '@') {
            string variableName = line.substr(1, line.size() - 1);
            bool isNum = (variableName[0] >= '0' && variableName[0] <= '9');
            bool isKnown = (symbolTable.count(variableName) > 0);
            
            if (!isNum && !isKnown) {
                symbolTable[variableName] = 16 + variableCount;
                variableCount++;
            }
        }
    }

    // print symbolTable
    for(unordered_map<string, int>::iterator i = symbolTable.begin(); i != symbolTable.end(); i++) {
        cout << i->first << " " << i->second << endl;
    }

    return 0;
}