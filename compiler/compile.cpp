#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <stdlib.h>

using namespace std;

class CompilationEngine {
    private:
    vector<string> tokens;
    vector<string>::iterator currentToken;
    stringstream xmlDump;
    string errorFile;
    string anzFile;
    int numSpaces;

    string getTokenType() {
        string token = *currentToken;
        return token.substr(1, token.find('>') - 1);
    }

    string getTokenContent() {
        string token = *currentToken;
        string tokenType = getTokenType(token);
        return token.substr(tokenType.length() + 3, token.length() - 2 * tokenType.length() - 7);
    }

    string getTokenType(string token) {
        return token.substr(1, token.find('>') - 1);
    }

    string getTokenContent(string token) {
        string tokenType = getTokenType(token);
        return token.substr(tokenType.length() + 3, token.length() - 2 * tokenType.length() - 6);
    }

    string generateSpaces() {
        return string(numSpaces, ' ');
    }

    bool isType() {
        string content = getTokenContent();
        return getTokenType() == "identifier" || (getTokenType() == "keyword" && (content == "int" || content == "char" || content == "boolean"));
    }

    bool isFunctionType() {
        string content = getTokenContent();
        return (getTokenType() == "keyword" && (content == "constructor" || content == "function" || content == "method"));
    }

    bool isOp() {
        string content = getTokenContent();
        return (getTokenType() == "symbol" && (content == "+" || content == "-" || content == "*" || content == "/" || content == "&amp;" || content == "|" || content == "&lt;" || content == "&gt;" || content == "="));
    }

    bool isUnaryOp() {
        string content = getTokenContent();
        return (getTokenType() == "symbol" && (content == "~" || content == "-"));
    }

    void incrementSpaces() {
        numSpaces += 2;
    }

    void decrementSpaces() {
        numSpaces -= 2;
    }

    void dump(string line) {
        xmlDump << generateSpaces() << line << "\n";
    }

    void dump() {
        xmlDump << generateSpaces() << *currentToken << "\n";
        currentToken++;
    }

    void writeError(string error) {
        cout << "Failed at " << "'" <<  *currentToken << "'" << endl << "'" << getTokenType() << "'" << endl << "'" <<  getTokenContent()  << "'" << endl << endl;
        cout << xmlDump.str() << endl;
        ofstream err(errorFile);
        err << error << endl;
        err.close();
        exit(EXIT_FAILURE);
    }

    void writeAnalysis() {
        ofstream anz(anzFile);
        anz << xmlDump.str();
        anz.close();
    }

    public:
    CompilationEngine(stringstream &tokenized, string anzPath, string errPath) {
        string token;
        while(getline(tokenized, token, '\n')) {
            tokens.push_back(token);
        }
        currentToken = ++tokens.begin();
        numSpaces = 0;

        anzFile = anzPath;
        errorFile = errPath;
        compileClass();

        writeAnalysis();
    }

    void compileClass() {
        if (getTokenType() == "keyword" && getTokenContent() == "class") {
            dump("<class>");
            incrementSpaces();
            dump();
        } else {
            writeError("Syntax error: 'class' expected.");
        }

        if (getTokenType() == "identifier") {
            dump();
        } else {
            writeError("Syntax error: identifier expected.");
        }

        if (getTokenType() == "symbol" && getTokenContent() == "{") {
            dump();
        } else {
            writeError("Syntax error: '{' expected.");
        }
        
        while (getTokenType() == "keyword" && (getTokenContent() == "static" || getTokenContent() == "field")) {
            compileClassVarDec();
        }

        while (getTokenType() == "keyword" && (getTokenContent() == "constructor" || getTokenContent() == "function" || getTokenContent() == "method")) {
            compileSubroutine();
        }

        if (getTokenType() == "symbol" && getTokenContent() == "}") {
            dump();
        } else {
            writeError("Syntax error: '}' expected.");
        }

        decrementSpaces();
        dump("</class>");
    }

    void compileClassVarDec() {
        dump("<classVarDec>");
        incrementSpaces();
        dump();

        if (isType()) {
            dump();
        } else {
            writeError("Syntax error: type expected.");
        }

        if (getTokenType() == "identifier") {
            dump();
        } else {
            writeError("Syntax error: identifier expected.");
        }

        if (getTokenType() == "identifier") {
            dump();
        } else {
            writeError("Syntax error: identifier expected.");
        }

        while (getTokenType() == "symbol" && getTokenContent() == ",") {
            dump();
            if (getTokenType() == "identifier") {
                dump();
            } else {
                writeError("Syntax error: identifier expected.");
            }
        }

        if (getTokenType() == "symbol" && getTokenContent() == ";") {
            dump();
        } else {
            writeError("Syntax error: ';' expected.");
        }

        decrementSpaces();
        dump("</classVarDec>");
    }

    void compileSubroutine() {
        dump("<subroutineDec>");
        incrementSpaces();

        if (isFunctionType()) {
            dump();
        } else {
            writeError("Syntax error: function type expected.");
        }

        if (getTokenType() == "keyword" && (getTokenContent() == "void" || isType())) {
            dump();
        } else {
            writeError("Syntax error: return type expected.");
        }

        if (getTokenType() == "identifier") {
            dump();
        } else {
            writeError("Syntax error: identifier expected.");
        }

        if (getTokenType() == "symbol" && getTokenContent() == "(") {
            dump();
        } else {
            writeError("Syntax error: '(' expected.");
        }

        compileParameterList();

        if (getTokenType() == "symbol" && getTokenContent() == ")") {
            dump();
        } else {
            writeError("Syntax error: ')' expected.");
        }

        dump("<subroutineBody>");
        incrementSpaces();

        if (getTokenType() == "symbol" && getTokenContent() == "{") {
            dump();
        } else {
            writeError("Syntax error: '{' expected.");
        }

        while (getTokenType() == "keyword" && getTokenContent() == "var") {
            dump("<varDec>");
            incrementSpaces();

            dump();
            if (isType()) {
                dump();
            } else {
                writeError("Syntax error: type expected.");
            }

            if (getTokenType() == "identifier") {
                dump();
            } else {
                writeError("Syntax error: identifier expected.");
            }

            while (getTokenType() == "symbol" && getTokenContent() == ",") {
                dump();
                if (getTokenType() == "identifier") {
                    dump();
                } else {
                    writeError("Syntax error: identifier expected.");
                }
            }

            if (getTokenType() == "symbol" && getTokenContent() == ";") {
                dump();
            } else {
                writeError("Syntax error: ';' expected.");
            }

            decrementSpaces();
            dump("</varDec>");
        }

        compileStatements();

        if (getTokenType() == "symbol" && getTokenContent() == "}") {
            dump();
        } else {
            writeError("Syntax error: '}' expected.");
        }

        decrementSpaces();
        dump("</subroutineBody>");

        decrementSpaces();
        dump("</subroutineDec>");
    }
    
    void compileParameterList() { 
        dump("<parameterList>");
        incrementSpaces();

        if (isType()) {
            dump();
            
            if (getTokenType() == "identifier") {
                dump();
            } else {
                writeError("Syntax error: identifier expected.");
            }

            while (getTokenType() == "symbol" && getTokenContent() == ",") {
                dump();
                
                if (isType()) {
                    dump();
                } else {
                    writeError("Syntax error: type expected.");
                }
                
                if (getTokenType() == "identifier") {
                    dump();
                } else {
                    writeError("Syntax error: identifier expected.");
                }
            }
        } else if (getTokenType() != "symbol" || getTokenContent() != ")") {
            writeError("Synax error: parameterList expected.");
        }

        decrementSpaces();
        dump("</parameterList>");
    }

    void compileStatements() {
        dump("<statements>");
        incrementSpaces();

        while(getTokenType() == "keyword") {
            string content = getTokenContent();
            if (content == "let") {
                compileLetStatement();
            } else if (content == "if") {
                compileIfStatement();
            } else if (content == "while") {
                compileWhileStatement();
            } else if (content == "do") {
                compileDoStatement();
            } else if (content == "return") {
                compileReturnStatement();
            } else {
                writeError("Syntax error: statement expected.");
            }
        }

        decrementSpaces();
        dump("</statements>");
    }

    void compileIfStatement() {
        dump("<ifStatement>");
        incrementSpaces();
        dump();

        if (getTokenType() == "symbol" && getTokenContent() == "(") {
            dump();
        } else {
            writeError("Syntax error: '(' expected.");
        }

        compileExpression();

        if (getTokenType() == "symbol" && getTokenContent() == ")") {
            dump();
        } else {
            writeError("Syntax error: ')' expected.");
        }

        if (getTokenType() == "symbol" && getTokenContent() == "{") {
            dump();
        } else {
            writeError("Syntax error: '{' expected.");
        }

        compileStatements();

        if (getTokenType() == "symbol" && getTokenContent() == "}") {
            dump();
        } else {
            writeError("Syntax error: '}' expected.");
        }

        if (getTokenType() == "keyword" && getTokenContent() == "else") {
            dump();

            if (getTokenType() == "symbol" && getTokenContent() == "{") {
                dump();
            } else {
                writeError("Syntax error: '{' expected.");
            }

            compileStatements();

            if (getTokenType() == "symbol" && getTokenContent() == "}") {
                dump();
            } else {
                writeError("Syntax error: '}' expected.");
            }
        }

        decrementSpaces();
        dump("</ifStatement>");
    }
    
    void compileReturnStatement() {
        dump("<returnStatement>");
        incrementSpaces();
        dump();

        string content = getTokenContent();
        if (getTokenType() == "integerConstant") {
            compileTerm();
        } else if (getTokenType() == "stringConstant") {
            compileTerm();
        } else if (getTokenType() == "keyword" && (content == "true" || content == "false" || content == "null" || content == "this")) {
            compileTerm();
        } else if (getTokenType() == "identifier") {
            compileTerm();
        } else if (getTokenType() == "symbol" && getTokenContent() == "(") {
            compileTerm();
        }
        

        if (getTokenType() == "symbol" && getTokenContent() == ";") {
            dump();
        } else {
            writeError("Syntax error: ';' expected.");
        }

        decrementSpaces();
        dump("</returnStatement>");
    }

    void compileWhileStatement() {
        dump("<whileStatement>");
        incrementSpaces();
        dump();

        if (getTokenType() == "symbol" && getTokenContent() == "(") {
            dump();
        } else {
            writeError("Syntax error: '(' expected.");
        }

        compileExpression();

        if (getTokenType() == "symbol" && getTokenContent() == ")") {
            dump();
        } else {
            writeError("Syntax error: ')' expected.");
        }

        if (getTokenType() == "symbol" && getTokenContent() == "{") {
            dump();
        } else {
            writeError("Syntax error: '{' expected.");
        }

        compileStatements();

        if (getTokenType() == "symbol" && getTokenContent() == "}") {
            dump();
        } else {
            writeError("Syntax error: '}' expected.");
        }

        decrementSpaces();
        dump("</whileStatement>");
    }

    void compileDoStatement() {
        dump("<doStatement>");
        incrementSpaces();
        dump();

        compileSubroutineCall();

        if (getTokenType() == "symbol" && getTokenContent() == ";") {
            dump();
        } else {
            writeError("Syntax error: ';' expected.");
        }

        decrementSpaces();
        dump("</doStatement>");
    }

    void compileLetStatement() {
        dump("<letStatement>");
        incrementSpaces();
        dump();

        if (getTokenType() == "identifier") {
            dump();
        } else {
            writeError("Syntax error: identifier expected.");
        }

        if (getTokenType() == "symbol" && getTokenContent() == "[") {
            dump();
            compileExpression();
            if (getTokenType() == "symbol" && getTokenContent() == "]") {
                dump();
            } else {
                writeError("Syntax error: ']' expected.");
            }
        }

        if (getTokenType() == "symbol" && getTokenContent() == "=") {
            dump();
        } else {
            writeError("Syntax error: '=' expected.");
        }

        compileExpression();

        if (getTokenType() == "symbol" && getTokenContent() == ";") {
            dump();
        } else {
            writeError("Syntax error: ';' expected.");
        }

        decrementSpaces();
        dump("</letStatement>");
    }

    void compileExpression() {
        dump("<expression>");
        incrementSpaces();

        compileTerm();

        while (isOp()) {
            dump();
            compileTerm();
        }


        decrementSpaces();
        dump("</expression>");
    }

    int compileExpressionList() {
        dump("<expressionList>");
        incrementSpaces();
        
        string content = getTokenContent();
        int numExpressions = 0;
        if (getTokenType() == "integerConstant") {
            numExpressions++;
            compileExpression();
        } else if (getTokenType() == "stringConstant") {
            numExpressions++;
            compileExpression();
        } else if (getTokenType() == "keyword" && (content == "true" || content == "false" || content == "null" || content == "this")) {
            numExpressions++;
            compileExpression();
        } else if (getTokenType() == "identifier") {
            numExpressions++;
            compileExpression();
        } else if (getTokenType() == "symbol" && getTokenContent() == "(") {
            numExpressions++;
            compileExpression();
        }
        
        while(getTokenType() == "symbol" && getTokenContent() == ",") {
            numExpressions++;
            dump();
            compileExpression();
        }

        decrementSpaces();
        dump("</expressionList>");

        return numExpressions;
    }
    
    void compileTerm() {
        dump("<term>");
        incrementSpaces();

        string content = getTokenContent();
        if (getTokenType() == "integerConstant") {
            dump();
        } else if (getTokenType() == "stringConstant") {
            dump();
        } else if (getTokenType() == "keyword" && (content == "true" || content == "false" || content == "null" || content == "this")) {
            dump();
        } else if (getTokenType() == "identifier") {
            dump();
            if (getTokenType() == "symbol" && getTokenContent() == "[") {
                dump();
                compileExpression();
                if (getTokenType() == "symbol" && getTokenContent() == "]") {
                    dump();
                } else {
                    writeError("Syntax error: ']' expected.");
                }
            } else if (getTokenType() == "symbol" && getTokenContent() == "(") {
                compileSubroutineCall(true);
            } else if (getTokenType() == "symbol" && getTokenContent() == ".") {
                dump();
                if (getTokenType() == "identifier") {
                    dump();
                } else {
                    writeError("Syntax error: identifier expected");
                }
                if (getTokenType() == "symbol" && getTokenContent() == "(") {
                    compileSubroutineCall(true);
                }
            }
        } else if (getTokenType() == "symbol" && getTokenContent() == "(") {
            dump();
            compileExpression();
            if (getTokenType() == "symbol" && getTokenContent() == ")") {
                dump();
            } else {
                writeError("Syntax error: ')' expected.");
            }
        } else {
            writeError("Syntax error: term expected.");
        }

        decrementSpaces();
        dump("</term>");
        
        
    }

    void compileSubroutineCall(bool identifierProvided = false) {
        if (!identifierProvided) {
            if (getTokenType() == "identifier") {
                dump();
            } else {
                writeError("Syntax error: identifier expected.");
            }
        

            if (getTokenType() == "symbol" && getTokenContent() == ".") {
                dump();
                if (getTokenType() == "identifier") {
                    dump();
                } else {
                    writeError("Syntax error: identifier expected.");
                }
            }
        }

        if (getTokenType() == "symbol" && getTokenContent() == "(") {
            dump();
        } else {
            writeError("Syntax error: '(' expected.");
        }

        compileExpressionList();

        if (getTokenType() == "symbol" && getTokenContent() == ")") {
            dump();
        } else {
            writeError("Syntax error: ')' expected.");
        }
    }

};

class TokenizationEngine {
    public:
    stringstream padSymbols(string original) {
        stringstream processed;
        bool inString = false;
        for(char c: original) {
            switch (c) {
                case '{':
                case '}':
                case '(':
                case ')':
                case '[':
                case ']':
                case '.':
                case ',':
                case ';':
                case '+':
                case '-':
                case '*':
                case '/':
                case '|':
                case '=':
                case '~':
                    if (!inString) processed << " " << c << " ";
                    else processed << c;
                    break;
                case '&':
                    if (!inString) processed << " &amp; ";
                    else processed << c;
                    break;
                case '<':
                    if (!inString) processed << " &lt; ";
                    else processed << " < ";
                    break;
                case '>':
                    if (!inString) processed << " &gt; ";
                    else processed << " > ";
                    break;
                case '"':
                    processed << c;
                    inString = !inString;
                    break;
                case '\t':
                    if (!inString) processed << " ";
                    else processed << c;
                    break;
                default:
                    processed << c;
                    break;
            }
        }

        string superPadded = processed.str();
        processed.str("");

        bool lastCharSpace = false;
        inString = false;
        for (char c: superPadded) {
            if (!inString) {
                if (c == ' ' && lastCharSpace) {
                    lastCharSpace = true;
                } else if (c != ' ' && lastCharSpace) {
                    processed << c;
                    lastCharSpace = false;
                } else if (c == ' ' && !lastCharSpace) {
                    processed << c;
                    lastCharSpace = true;
                } else if (c !=  ' ' && !lastCharSpace) {
                    processed << c;
                    lastCharSpace = false;
                }
            } else {
                processed << c;
                lastCharSpace = false;
            }
            if (c == '"') inString = !inString;
        }
        
        return processed;
    }

    string createToken(string atomic) {
        if (atomic == "{" || atomic == "}" || atomic == "(" || atomic == ")" || atomic == "[" || 
            atomic == "]" || atomic == "." || atomic == "," || atomic == ";" || atomic == "+" || 
            atomic == "-" || atomic == "*" || atomic == "/" || atomic == "&amp;" || atomic == "|" || 
            atomic == "&lt;" || atomic == "&gt;" || atomic == "=" || atomic == "~") {
            
            return "<symbol> " + atomic + " </symbol>\n";
        } else if (atomic[0] == '"' && atomic.back() == '"') {
            return "<stringConstant> " + atomic.substr(1, atomic.length() - 2) + " </stringConstant>\n";
        } else if (atomic[0] >= '0' && atomic[0] <= '9') {
            return "<integerConstant> " + atomic + " </integerConstant>\n";
        } else if (atomic == "class" || atomic == "constructor" || atomic == "function" || 
            atomic == "method" || atomic == "field" || atomic == "static" || atomic == "var" ||
            atomic == "int" || atomic == "char" || atomic == "boolean" || atomic == "void" || atomic == "true" ||
            atomic == "false" || atomic == "null" || atomic == "this" || atomic == "let" || atomic == "do" ||
            atomic == "if" || atomic == "else" || atomic == "while" || atomic == "return") {
            
            return "<keyword> " + atomic + " </keyword>\n";
        } else {
            return "<identifier> " + atomic + " </identifier>\n";
        }
    } 
};

int main(int argc, char** argv) {

    int numFiles;

    // make sure command is executed properly
    if (argc < 3) {
        cerr << "ERROR: Provide appropriate command line arguments" << endl;
        return 1;
    } else {
        numFiles = stoi(argv[1]);
        if (argc != numFiles + 2) {
            cerr << "ERROR: Provide appropriate command line arguments" << endl;
            return 1;
        }
    }

    stringstream tokenized;
    tokenized << "<tokens>\n";

    for(int fileNumber = 2; fileNumber < argc; fileNumber++) {
        string filename = argv[fileNumber];
        string jackFileContents;
        
        // read file contents into jackFileContents
        if (ifstream jackFile = ifstream(filename.c_str())) {
            jackFileContents.assign((istreambuf_iterator<char>(jackFile)), (istreambuf_iterator<char>()));
        } else {
            cerr << "ERROR: File " + filename +  " not found" << endl;
            return 1;
        }

        // remove all comments, whitespace, multi-newlines from jackFileContents
        regex anyComment("(\\/\\/.*|\\/\\*[\\s\\S]*?\\*\\/)");
        regex newline("[\\r\\n]+");
        regex whitespaceNewline(" \\n");
        regex whitespace("[ ]+");

        jackFileContents = regex_replace(jackFileContents, anyComment, "");
        jackFileContents = regex_replace(jackFileContents, whitespace, " ");
        jackFileContents = regex_replace(jackFileContents, newline, "\n");
        jackFileContents = regex_replace(jackFileContents, whitespaceNewline, "\n");
        if (jackFileContents[0] == '\n' || jackFileContents[0] == '\r') jackFileContents.erase(jackFileContents.begin());
        if (jackFileContents[0] == '\n' || jackFileContents[0] == '\r') jackFileContents.erase(jackFileContents.begin());
        if (jackFileContents.back() == '\n' || jackFileContents.back() == '\r') jackFileContents.erase(jackFileContents.end() - 1);
        if (jackFileContents.back() == '\n' || jackFileContents.back() == '\r') jackFileContents.erase(jackFileContents.end() - 1);

        TokenizationEngine te;
        stringstream jackStream = te.padSymbols(jackFileContents);
        //cout << jackStream.str() << endl;
        string atomic, word;
        bool inString = false;
        while (jackStream >> word) {
            if (inString) {
                atomic = atomic + " " + word;
                if (word.back() == '"') {
                    inString = false;
                    tokenized << te.createToken(atomic);
                }
            } else {
                atomic = word;
                if (word[0] != '"') {
                    tokenized << te.createToken(atomic);
                } else {
                    atomic = word;
                    inString = true;
                }
            }
        }
        
    }
    tokenized << "</tokens>\n";

    ofstream outFile("tokens.xml");
    outFile << tokenized.str();
    outFile.close();

    CompilationEngine ce(tokenized, "out.anz", "out.err");

    return 0;
};