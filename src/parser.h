#include <vector>
#include <memory>
#include "tokens.h"
#include "ast.h"
#include "lexer.h"

using namespace std;

class Parser{
private:
    int current=0;
    vector<Token> tokens;

    Token peek();
    Token previous();

    bool is_at_end();

    Token advance();

    bool check(TokenType type);
    bool match(vector<TokenType> types);

    Token consume(TokenType type, string mess);


    unique_ptr<Expr> primary();
    unique_ptr<Expr> call();
    unique_ptr<Expr> unary();
    unique_ptr<Expr> factor();
    unique_ptr<Expr> term();
    unique_ptr<Expr> comparision();
    unique_ptr<Expr> equality();
    unique_ptr<Expr> logic_and();
    unique_ptr<Expr> logic_or();
    unique_ptr<Expr> assignment();
    unique_ptr<Expr> expression();



    unique_ptr<Stmt> print_statement();
    unique_ptr<Stmt> expression_statement();
    unique_ptr<Stmt> var_declaration();
    vector<unique_ptr<Stmt>> block();
    unique_ptr<Stmt> if_statement();
    unique_ptr<Stmt> while_statement();
    unique_ptr<Stmt> for_statement();
    unique_ptr<Stmt> fun_declaration();
    unique_ptr<Stmt> return_statement();
    unique_ptr<Stmt> statement();
    unique_ptr<Stmt> declaration();



    // unique_ptr<Stmt> parse();





public:
    Parser(vector<Token> t){
        tokens=t;
    }

    vector<unique_ptr<Stmt>> parse();

};