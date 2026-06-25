
#include <iostream>
#include <fstream>
#include <sstream>

// #include "lexer.h"
// #include "ast.h"
#include "parser.h"
#include "environment.h"
#include "interpreter.h"

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

    Environment e;
    Value v=5.5;
    e.define("x",v);
    
    // Value k=e.get(Token(TokenType::IDENTIFIER,"x",0));

    Environment m(make_shared<Environment>(e));

    Value d=m.get(Token(TokenType::IDENTIFIER,"x",0));

    double *dptr=get_if<double>(&d);
    cout<<*dptr<<"\n\n";
    Interpreter i;
    i.interpret(statements);

    return 0;

}