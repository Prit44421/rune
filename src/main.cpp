#include <iostream>
#include <fstream>
#include <sstream>

// #include "lexer.h"
// #include "ast.h"
#include "parser.h"

using namespace std;

int main(int argc, char* argv[]){


    
    if(argc<2){
        cout<<"ERROR: No file provided\nUsage: rune <script.rune>\n";
        return 1;
    }
    
    ifstream file(argv[1]);

    if(!file ){
        cout<<"ERROR: File not found\n";
        return 1;
    }

    stringstream buff;
    buff<<file.rdbuf();
    string content=buff.str();

    Lexer lexer(content);

    vector<Token> tokens = lexer.scan_tokens();

    cout<<"TokenType :: Value :: Line no\n";
    for(auto i:tokens){
        i.print_token();
    }

    Parser parser(tokens);

    vector<unique_ptr<Stmt>> statements = parser.parse();

    cout<<"Parsed Statements: "<<statements.size()<<"\n";


    return 0;

}