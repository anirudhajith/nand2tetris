#include <iostream>
#include <regex>
#include <fstream>
#include <string>

using namespace std;

inline string getClassname() {
    return "Xxx";
}

string getAssembly(string vmInstruction) {
    static int instuctionIndex = 0;
    instuctionIndex++;

    if (vmInstruction == "add") {
        return "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "M=M+D\n";
    } else if (vmInstruction == "sub") {
        return "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "M=M-D\n";
    } else if (vmInstruction == "and") {
        return "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "M=M&D\n";
    } else if (vmInstruction == "or") {
        return "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "M=M|D\n";
    } else if (vmInstruction == "not") {
        return "@SP\n"
                "A=M-1\n"
                "M=!M\n";
    } else if (vmInstruction == "neg") {
        return "@SP\n"
                "A=M-1\n"
                "M=!M\n";
    } else if (vmInstruction == "eq") {
        return "@SP\n"
                "AM=M-1\n"
                "D=M+1\n"
                "A=A-1\n"
                "MD=M-D\n"
                "@IFEQ" + to_string(instuctionIndex) + "\n"
                "D;JEQ\n"
                "@SP\n"
                "A=M-1\n"
                "M=0\n"
                "(IFEQ" + to_string(instuctionIndex) + ")\n";
    } else if (vmInstruction == "lt") {
        return "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "M=0\n"
                "@IFNLT" + to_string(instuctionIndex) + "\n"
                "D;JGE\n"
                "@SP\n"
                "A=M-1\n"
                "M=-1\n"
                "(IFNLT" + to_string(instuctionIndex) + ")\n";
    } else if (vmInstruction == "gt") {
        return "@SP\n"
                "AM=M-1\n"
                "D=M\n"
                "A=A-1\n"
                "D=M-D\n"
                "M=0\n"
                "@IFNGT" + to_string(instuctionIndex) + "\n"
                "D;JLE\n"
                "@SP\n"
                "A=M-1\n"
                "M=-1\n"
                "(IFNGT" + to_string(instuctionIndex) + ")\n";
    } else if (vmInstruction.substr(0,5) == "push ") {
        if (vmInstruction.substr(5,6) == "local ") {
            return "@LCL\n"
                    "D=M\n"
                    "@" + vmInstruction.substr(11) + "\n"
                    "A=A+D\n"
                    "D=M\n"
                    "@SP\n"
                    "M=M+1\n"
                    "A=M-1\n"
                    "M=D\n";
        } else if (vmInstruction.substr(5,9) == "argument ") {
            return "@ARG\n"
                    "D=M\n"
                    "@" + vmInstruction.substr(14) + "\n"
                    "A=A+D\n"
                    "D=M\n"
                    "@SP\n"
                    "M=M+1\n"
                    "A=M-1\n"
                    "M=D\n";
        } else if (vmInstruction.substr(5,5) == "this ") {
            return "@THIS\n"
                    "D=M\n"
                    "@" + vmInstruction.substr(10) + "\n"
                    "A=A+D\n"
                    "D=M\n"
                    "@SP\n"
                    "M=M+1\n"
                    "A=M-1\n"
                    "M=D\n";
        } else if (vmInstruction.substr(5,5) == "that ") {
            return "@THAT\n"
                    "D=M\n"
                    "@" + vmInstruction.substr(10) + "\n"
                    "A=A+D\n"
                    "D=M\n"
                    "@SP\n"
                    "M=M+1\n"
                    "A=M-1\n"
                    "M=D\n";
        } else if (vmInstruction.substr(5,9) == "constant ") {
            return "@" + vmInstruction.substr(14) + "\n"
                    "D=A\n"
                    "@SP\n"
                    "M=M+1\n"
                    "A=M-1\n"
                    "M=D\n";
        } else if (vmInstruction.substr(5,8) == "pointer ") {
            if (vmInstruction.substr(13) == "0") {
                return "@THIS\n"
                        "D=M\n"
                        "@SP\n"
                        "M=M+1\n"
                        "A=M-1\n"
                        "M=D\n";
            } else if (vmInstruction.substr(13) == "1") {
                return "@THAT\n"
                        "D=M\n"
                        "@SP\n"
                        "M=M+1\n"
                        "A=M-1\n"
                        "M=D\n";
            }
        } else if (vmInstruction.substr(5,5) == "temp ") {
            int index = stoi(vmInstruction.substr(10));
            if (index > 0 && index < 8) {
                return "@" + to_string(index + 5) + "\n"
                        "D=M\n"
                        "@SP\n"
                        "M=M+1\n"
                        "A=M-1\n"
                        "M=D\n";
            } else {
                return "ERROR: UNKNOWN INSTRUCTION";
            }
        } else if (vmInstruction.substr(5,7) == "static ") {
            return "@" + getClassname() + "." + vmInstruction.substr(12) + "\n"
                    "D=M\n"
                    "@SP\n"
                    "M=M+1\n"
                    "A=M-1\n"
                    "M=D\n";
        } else {
            return "ERROR: UNKNOWN INSTRUCTION";
        }
    } else if (vmInstruction.substr(0,4) == "pop ") {
        if (vmInstruction.substr(4,6) == "local ") {
            return "@LCL\n"
                    "D=M\n"
                    "@" + vmInstruction.substr(10) + "\n"
                    "D=A+D\n"
                    "@SP\n"
                    "AM=M-1\n"
                    "D=D+M\n"
                    "A=D-M\n"
                    "M=D-A\n";
        } else if (vmInstruction.substr(4,9) == "argument ") {
            return "@ARG\n"
                    "D=M\n"
                    "@" + vmInstruction.substr(13) + "\n"
                    "D=A+D\n"
                    "@SP\n"
                    "AM=M-1\n"
                    "D=D+M\n"
                    "A=D-M\n"
                    "M=D-A\n";
        } else if (vmInstruction.substr(4,5) == "this ") {
            return "@THIS\n"
                    "D=M\n"
                    "@" + vmInstruction.substr(9) + "\n"
                    "D=A+D\n"
                    "@SP\n"
                    "AM=M-1\n"
                    "D=D+M\n"
                    "A=D-M\n"
                    "M=D-A\n";
        } else if (vmInstruction.substr(4,5) == "that ") {
            return "@THAT\n"
                    "D=M\n"
                    "@" + vmInstruction.substr(9) + "\n"
                    "D=A+D\n"
                    "@SP\n"
                    "AM=M-1\n"
                    "D=D+M\n"
                    "A=D-M\n"
                    "M=D-A\n";
        } else if (vmInstruction.substr(4,8) == "pointer ") {
            if (vmInstruction.substr(12) == "0") {
                return "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "@THIS\n"
                        "M=D\n";
            } else if (vmInstruction.substr(12) == "1") {
                return "@SP\n"
                        "AM=M-1\n"
                        "D=M\n"
                        "@THAT\n"
                        "M=D\n";
            }
        } else if (vmInstruction.substr(4,5) == "temp ") {
            int index = stoi(vmInstruction.substr(9));
            if (index > 0 && index < 8) {
                return  "@" + to_string(index + 5) + "\n"
                        "D=A\n"
                        "@SP\n"
                        "AM=M-1\n"
                        "D=D+M\n"
                        "A=D-M\n"
                        "M=D-A\n";
            } else {
                return "ERROR: UNKNOWN INSTRUCTION";
            }
        } else if (vmInstruction.substr(4,7) == "static ") {
            return "@SP\n"
                    "AM=M-1\n"
                    "D=M\n"
                    "@" + getClassname() + "." + vmInstruction.substr(11) + "\n"
                    "M=D\n";
        }
    } else {
        return "ERROR: UNKNOWN INSTRUCTION";
    }
}

int main(int argc, char** argv) {
    
    // make sure command is executed properly
    if (argc == 1) {
        cerr << "ERROR: Provide vm file as command line argument" << endl;
        return 1;
    } else if (argc > 2) {
        cerr << "ERROR: Provide only one vm file as command line argument" << endl;
        return 1;
    }

    string filename = argv[1];
    string vmFileContents;
    
    // read file contents into vmFileContents
    if (ifstream vmFile = ifstream(filename.c_str())) {
        vmFileContents.assign((istreambuf_iterator<char>(vmFile)), (istreambuf_iterator<char>()));
        vmFile.close();
    } else {
        cerr << "ERROR: File not found" << endl;
        return 1;
    }

    // remove all comments, whitespace, multi-newlines from vmFileContents
    regex anyComment("(\\/\\/.*|\\/\\*[\\s\\S]*?\\*\\/)");
    regex newline("[\\r\\n]+");
    //regex whitespace("[     ]");

    vmFileContents = regex_replace(vmFileContents, anyComment, "");
    //vmFileContents = regex_replace(vmFileContents, whitespace, "");
    vmFileContents = regex_replace(vmFileContents, newline, "\n");
    if (vmFileContents[0] == '\n' || vmFileContents[0] == '\r') vmFileContents.erase(vmFileContents.begin());
    if (vmFileContents[0] == '\n' || vmFileContents[0] == '\r') vmFileContents.erase(vmFileContents.begin());
    if (vmFileContents.back() == '\n' || vmFileContents.back() == '\r') vmFileContents.erase(vmFileContents.end() - 1);
    if (vmFileContents.back() == '\n' || vmFileContents.back() == '\r') vmFileContents.erase(vmFileContents.end() - 1);

    istringstream vmStream(vmFileContents);
    stringstream asmStream;
    for(string line; getline(vmStream, line);) {
        string asmString = getAssembly(line);
        if (asmString != "ERROR: UNKNOWN INSTRUCTION") {
            asmStream << asmString;
        } else {
            cerr << "ERROR: UNKNOWN INSTRUCTION" << endl << line << endl;
            return 1;
        }
    }

    // save machine code into assembler.out
    ofstream outFile("out.asm");
    outFile << asmStream.rdbuf();
    outFile.close();

    return 0;
}