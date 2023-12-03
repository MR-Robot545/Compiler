#include <bits/stdc++.h>
#include <fstream>
#include <vector>
#include <cctype>

using namespace std;

// Enum for token types
enum TokenType {
    ID, // Identifier
    NUM, // Number
    IF, // if
    THEN, // then
    REPEAT, // repeat
    UNTIL, // until
    READ, // read
    WRITE, // write
    ASSIGN, // :=
    PLUS, // +
    MINUS, // -
    TIMES, // *
    DIVIDE, // /
    EQ, // =
    LT, // <
    LPAREN, // (
    RPAREN, // )
    LBRACE, // {
    RBRACE, // }
    SEMI, // ;
    END, // end
    ERROR, // Error token
    START,
    POWER,
    ELSE
};
// Struct to represent a token
struct Token {
    int lineNumber;
    string lexeme;
    TokenType type;

};

class Scanner {
public:
    Scanner(const string& filename);
    vector<Token> getTokens();
    void printTokensToFile(const string& outputFilename);

private:
    ifstream inputFile;
    vector<Token> tokens;
    int lineNumber;

    // Utility functions
    char getNextChar();
    void ungetChar(char c);
    void skipWhitespace();
    void scanIdentifier(char firstChar);
    void scanNumber(char firstChar);
};

// Constructor
Scanner::Scanner(const string& filename) : lineNumber(1) {
    inputFile.open(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
}

// Get the next character from the input file
char Scanner::getNextChar() {
    char c = inputFile.get();
    if (c == '\n') {
        lineNumber++;
    }
    return c;
}

// Put back the last character to the input file
void Scanner::ungetChar(char c) {
    inputFile.unget();
    if (c == '\n') {
        lineNumber--;
    }
}

// Skip whitespace characters
void Scanner::skipWhitespace() {
    char c;
    while ((c = getNextChar()) != EOF && isspace(c)) {
        // Skip whitespace
    }
    ungetChar(c);
}

// Scan an identifier
void Scanner::scanIdentifier(char firstChar) {
    string lexeme;
    lexeme += firstChar;

    char c;
    while ((c = getNextChar()) != EOF && isalnum(c) || c == '_') {
        lexeme += c;
    }
    ungetChar(c);

    TokenType type;

    // Check if it is a keyword
    if (lexeme == "if") {
        type = IF;
    } else if (lexeme == "then") {
        type = THEN;
    } else if (lexeme == "repeat") {
        type = REPEAT;
    } else if (lexeme == "until") {
        type = UNTIL;
    } else if (lexeme == "read") {
        type = READ;
    } else if (lexeme == "write") {
        type = WRITE;
    } else if (lexeme == "end") {
        type = END;
    } else  if(lexeme=="else"){
        type=ELSE;
    }else{
        type = ID;
    }

    tokens.push_back({lineNumber, lexeme, type});
}

// Scan a number
void Scanner::scanNumber(char firstChar) {
    string lexeme;
    lexeme += firstChar;

    char c;
    while ((c = getNextChar()) != EOF && isdigit(c)) {
        lexeme += c;
    }
    ungetChar(c);

    tokens.push_back({lineNumber, lexeme, NUM});
}

// Function to convert TokenType to string
string tokenTypeToString(TokenType type) {
    switch (type) {
        case ID:
            return "ID";
        case NUM:
            return "NUM";
        case IF:
            return "IF";
        case THEN:
            return "THEN";
        case REPEAT:
            return "REPEAT";
        case UNTIL:
            return "UNTIL";
        case READ:
            return "READ";
        case WRITE:
            return "WRITE";
        case ASSIGN:
            return "ASSIGN";
        case PLUS:
            return "PLUS";
        case MINUS:
            return "MINUS";
        case TIMES:
            return "TIMES";
        case DIVIDE:
            return "DIVIDE";
        case EQ:
            return "EQ";
        case LT:
            return "LT";
        case LPAREN:
            return "LPAREN";
        case RPAREN:
            return "RPAREN";
        case LBRACE:
            return "LBRACE";
        case RBRACE:
            return "RBRACE";
        case SEMI:
            return "SEMI";
        case END:
            return "END";
        case ERROR:
            return "ERROR";
        case POWER:
            return "POWER";
        case ELSE:
            return "ELSE";
        case START:
            return "START";
        default:
            return "UNKNOWN";
    }
}

// Get the list of tokens
vector<Token> Scanner::getTokens() {
    char c;
    while ((c = getNextChar()) != EOF) {
        if (isspace(c)) {
            // Skip whitespace
            skipWhitespace();
        } else if (isalpha(c) || c == '_') {
            // Identifier
            scanIdentifier(c);
        } else if (isdigit(c)) {
            // Number
            scanNumber(c);
        } else {
            // Handle other characters
            switch (c) {
                case ':':
                    if ((c = getNextChar()) == '=') {
                        tokens.push_back({lineNumber, ":=", ASSIGN});
                    } else {
                        ungetChar(c);
                        tokens.push_back({lineNumber, ":", ERROR});
                    }
                    break;
                case '+':
                    tokens.push_back({lineNumber, "+", PLUS});
                    break;
                case '-':
                    tokens.push_back({lineNumber, "-", MINUS});
                    break;
                case '*':
                    tokens.push_back({lineNumber, "*", TIMES});
                    break;
                case '/':
                    tokens.push_back({lineNumber, "/", DIVIDE});
                    break;
                case '=':
                    tokens.push_back({lineNumber, "=", EQ});
                    break;
                case '<':
                    tokens.push_back({lineNumber, "<", LT});
                    break;
                case '(':
                    tokens.push_back({lineNumber, "(", LPAREN});
                    break;
                case ')':
                    tokens.push_back({lineNumber, ")", RPAREN});
                    break;
                case ';':
                    tokens.push_back({lineNumber, ";", SEMI});
                    break;
                case '{':
                    tokens.push_back({lineNumber, "{", LBRACE});
                    // Comment, skip until closing brace '}'
                    while ((c = getNextChar()) != EOF && c != '}') {
                        // Skip comment
                    }
                    tokens.push_back({lineNumber, "}", RBRACE});
                    break;
                case '^':
                    tokens.push_back({lineNumber, "^", POWER});
                    break;
                default:
                    tokens.push_back({lineNumber, string(1, c), ERROR});
                    break;
            }
        }
    }

    return tokens;
}

// Print tokens to a file
void Scanner::printTokensToFile(const string& outputFilename) {
    ofstream outputFile(outputFilename);
    if (!outputFile.is_open()) {
        cerr << "Error opening output file: " << outputFilename << endl;
        exit(EXIT_FAILURE);
    }

    for (const Token& token : tokens) {
        outputFile << "[" << token.lineNumber << "] " << token.lexeme << " (" << tokenTypeToString(token.type) << ")" << endl;
    }

    outputFile.close();
}
// content of each node
struct Node{
    string val;
    TokenType type;
};
// tree of nodes and childes
struct parse{
    Node node;
    vector<parse>child;
public:
    parse(){};
    parse(Token& t){
        node.val=t.lexeme;
        node.type=t.type;
    }
    parse(string& str,TokenType& tt){
        node.val=str;
        node.type=tt;
    }
};
// component of the parse tree
struct Tree{
public:
    // skip the brackets of the comments
    void checkparanthes(vector<Token> &token) {
        while (!token.empty()&&token.back().type == LBRACE || token.back().type == RBRACE) {
            token.pop_back();
        }
    }
//    newexp->( mathexpr ) | number | identifier
    parse newexp(vector<Token>&token){
        parse node;
        if(!token.empty()){
            if(token.back().type==NUM||token.back().type==ID){
                node=parse(token.back());
                token.pop_back();
                return node;
            }else  if(token.back().type==LBRACE){
                while (token.back().type==LBRACE)token.pop_back();
                node=math_expression(token);
                while (token.back().type==RBRACE)token.pop_back();
                return node;
            }else{
                cerr<<"error"<<endl;
                exit(0);
            }
        }
        checkparanthes(token);
    }
    // factor -> newexpr { ^ newexpr }
    parse factor(vector<Token>&token){
        parse op=newexp(token);
        if (!token.empty()&&token.back().type==POWER){
            parse node;
            node=parse(token.back());
            token.pop_back();
            node.child.push_back(op);
            node.child.push_back(factor(token));
            return node;
        }
        checkparanthes(token);
        return op;
    }
//    term -> factor { (* | /) factor }
    parse term(vector<Token>&token){
        parse op=factor(token);
        while (!token.empty()){
            if (!token.empty()&&(token.back().type==TIMES||token.back().type==DIVIDE)){
                parse node;
                node=parse(token.back());
                token.pop_back();
                node.child.push_back(op);
                parse temp;
                node.child.push_back(factor(token));
                op=node;
            }else{
                break;
            }
        }
        checkparanthes(token);
        return op;
    }
//    mathexpr -> term { (+| -) term }
    parse math_expression(vector<Token>&token){
        parse op=term(token);
        while (!token.empty()){
            if (!token.empty()&&(token.back().type==PLUS||token.back().type==MINUS)){
                parse node;
                node=parse(token.back());
                token.pop_back();
                node.child.push_back(op);
                parse temp;
                node.child.push_back(term(token));
                op=node;
            }else{
                break;
            }
        }
        checkparanthes(token);
        return op;
    }
//    expr -> mathexpr [ (< | =) mathexpr ]
    parse expression(vector<Token>&token){
        parse op=math_expression(token);
        if (!token.empty()&&(token.back().type==LT||token.back().type==EQ)){
            parse node,temp;
            node=parse(token.back());
            token.pop_back();
            node.child.push_back(op);
            node.child.push_back(math_expression(token));
            return node;
        }
        checkparanthes(token);
        return op;
    }

//    readstmt -> read identifier
    parse readstat(vector<Token> &token) {
        parse root;
        if(!token.empty()&&token.back().type==READ){
            root=parse(token.back());
            token.pop_back();
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        // ID
        parse temp;
        if(!token.empty()&&token.back().type==ID){
            temp=parse(token.back());
            root.child.push_back(temp);
            token.pop_back();
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        // semi clone
        if(!token.empty()&&token.back().type==SEMI){
            temp=parse(token.back());
            root.child.push_back(temp);
            token.pop_back();
        }
        checkparanthes(token);
        return root;
    }
//    writestmt -> write expr
    parse writestat(vector<Token> &token) {
        parse root;
        if(!token.empty()&&token.back().type==WRITE){
            root=parse(token.back());
            token.pop_back();
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        root.child.push_back(expression(token));
        parse temp;
        if(!token.empty()&&token.back().type==SEMI){
            temp=parse(token.back());
            root.child.push_back(temp);
            token.pop_back();
        }
        checkparanthes(token);
        return root;
    }
//    ifstmt -> if expr then stmtseq [ else stmtseq ] end
    parse ifstat(vector<Token> &token) {
        parse root;
        if(!token.empty()&&token.back().type==IF){
            root=parse(token.back());
            token.pop_back();
            root.child.push_back(expression(token));
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        // read then
        if(!token.empty()&&token.back().type==THEN){
            root.child.emplace_back(token.back());
            token.pop_back();
            while (!token.empty()&&token.back().type!=END&&!token.empty()&&token.back().type!=ELSE)
                root.child.back().child.push_back(statment(token));
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        // read else
        if(!token.empty()&&token.back().type==ELSE){
            root.child.emplace_back(token.back());
            token.pop_back();
            while (!token.empty()&&token.back().type!=END)
                root.child.back().child.push_back(statment(token));
        }
        // read end
        if(!token.empty()&&token.back().type==END){
            root.child.emplace_back(token.back());
            token.pop_back();
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        checkparanthes(token);
        return root;
    }

//    repeatstmt -> repeat stmtseq until expr
    parse repeatstat(vector<Token> &token) {
        parse root;
        if(token.back().type==REPEAT){
            root=token.back();
            token.pop_back();
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        while (!token.empty()&&token.back().type!=UNTIL)
            root.child.push_back(statment(token));
        if(token.back().type==UNTIL){
            root.child.emplace_back(token.back());
            token.pop_back();
            root.child.back().child.push_back(expression(token));
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        if(!token.empty()&&token.back().type==SEMI){
            root.child.back().child.emplace_back(token.back());
            token.pop_back();
        }
        checkparanthes(token);
        return root;
    }
//    assignstmt -> identifier := expr
    parse assignmentstat(vector<Token> &token) {
        parse root;
        if(!token.empty()&&token.back().type==ID){
            root.node.val=token.back().lexeme;
            token.pop_back();
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        if(!token.empty()&&token.back().type==ASSIGN){
            root.node.type=token.back().type;
            token.pop_back();
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }
        root.child.push_back(expression(token));
        if(!token.empty()&&token.back().type==SEMI){
            root.child.emplace_back(token.back());
            token.pop_back();
        }
        return root;
    }

//    stmt -> ifstmt | repeatstmt | assignstmt | readstmt | writestmt
    parse statment(vector<Token>&token){
        checkparanthes(token);
        parse temp;
        if(!token.empty()&&token.back().type==IF){
            temp=ifstat(token);
        }else if(!token.empty()&&token.back().type==REPEAT){
            temp=repeatstat(token);
        }else if(!token.empty()&&token.back().type==READ){
            temp=readstat(token);
        }else if(!token.empty()&&token.back().type==ID){
            temp=assignmentstat(token);
        }else if(!token.empty()&&token.back().type==WRITE){
            temp= writestat(token);
        }else{
            cerr<<"error"<<endl;
            exit(0);
        }

        return temp;
    }
//    stmtseq -> stmt { ; stmt }
    parse statseq(vector<Token>&token){
        Token prog;
        prog.lexeme="Program";
        prog.type=START;
        parse root(prog);
        while (!token.empty()){
            checkparanthes(token);
            root.child.push_back(statment(token));
        }
        return root;
    }
    // print the parse tree
    void print(parse& root,int depth){
        for (int i = 0; i <depth*3 ; ++i) {
            cout<<" ";
        }
        cout<<"["<<root.node.val<<"]["<<tokenTypeToString(root.node.type)<<"]"<<endl;
        for(auto& u:root.child){
            print(u,depth+1);
        }

    }
};
int main() {
    // Change the input file name accordingly
    Scanner scanner("in.txt");

    // Get the list of tokens
    vector<Token> tokens = scanner.getTokens();
    Tree t;
    //reverse the vector of tokens to start from the back
    reverse(tokens.begin(),tokens.end());
    parse Root=t.statseq(tokens);
    t.print(Root,0);
//     Change the output file name accordingly
    scanner.printTokensToFile("out.txt");

    return 0;
}






