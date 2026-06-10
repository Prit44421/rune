#include "parser.h"
// #include "tokens.h"


Token Parser::peek(){

    return tokens[current];
 
}

Token Parser::previous(){
    return tokens[current-1];
}

bool Parser::is_at_end(){
    return peek().type==TokenType::END_OF_FILE;
}

Token Parser::advance(){
    if(!is_at_end()){
        return tokens[current++];
    }
    // what should i add here
    cout<<"Error: Reached end of file\n";
    return tokens[current++];
}


bool Parser::check(TokenType t){
    return peek().type==t;
}

bool Parser::match(vector<TokenType> t){
    for(TokenType token:t){
        if(check(token)){
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::consume(TokenType t, string mess){
    if(check(t)) return advance();
    cout<<"Error: "<<mess<<" at line: "<<peek().line<<"\n"; //todo want to add line here 
    return previous(); // todo what to do 
}

unique_ptr<Expr> Parser::primary(){
    if(match({TokenType::NUMBER, TokenType::STRING})){
        return make_unique<Literal>(previous().text); 
    }
    else if(match({TokenType::TRUE})){
        return make_unique<Literal>(true); 
    }
    else if(match({TokenType::FALSE})){
        return make_unique<Literal>(false); 
    }
    else if(match({TokenType::NIL})){
        return make_unique<Literal>(Nil{}); 
    }
    else if(match({TokenType::IDENTIFIER})){
        return make_unique<VarExpr>(previous()); 
    }
    else if(match({TokenType::LEFT_PAREN_CURVE})){
        unique_ptr<Expr> e=expression();
        consume(TokenType::RIGHT_PAREN_CURVE, "Need ) after expr");
        return make_unique<GroupExpr>(move(e));
    }
    else{
        // todo error
        cout<<"Error(primary) line: "<<peek().line<<"\n";
        return nullptr; // wat ??
    }

}

unique_ptr<Expr> Parser::call(){
    return primary();
    // todo add funtion parser
}

unique_ptr<Expr> Parser::unary(){
    if(match({TokenType::NOT, TokenType::MINUS})){
        Token op=previous();
        unique_ptr<Expr> u=unary();
        return make_unique<UnaryExpr>(op,move(u));
    }
    return call();
}

unique_ptr<Expr> Parser::factor(){
    unique_ptr<Expr> l=unary();
    while(match({TokenType::STAR,TokenType::SLASH,TokenType::PERCENT})){
        Token op=previous();
        unique_ptr<Expr> u=unary();
        l=make_unique<BinaryExpr>(move(l),op,move(u));
    }
    return l;
}


unique_ptr<Expr> Parser::term(){
    unique_ptr<Expr> l=factor();
    while(match({TokenType::PLUS,TokenType::MINUS})){
        Token op=previous();
        unique_ptr<Expr> f=factor();
        l=make_unique<BinaryExpr>(move(l),op,move(f));
    }
    return l;
}

unique_ptr<Expr> Parser::comparision(){
    unique_ptr<Expr> l=term();
    while(match({TokenType::GREATER,TokenType::GREATER_EQUAL,TokenType::LESS,TokenType::LESS_EQUAL})){
        Token op=previous();
        unique_ptr<Expr> t=term();
        l=make_unique<BinaryExpr>(move(l),op,move(t));
    }
    return l;
}


unique_ptr<Expr> Parser::equality(){
    unique_ptr<Expr> l=comparision();
    while(match({TokenType::EQUAL_EQUAL,TokenType::NOT_EQUAL})){
        Token op=previous();
        unique_ptr<Expr> c=comparision();
        l=make_unique<BinaryExpr>(move(l),op,move(c));
    }
    return l;
}


unique_ptr<Expr> Parser::logic_and(){
    unique_ptr<Expr> l=equality();
    while(match({TokenType::AND})){
        unique_ptr<Expr> r=equality();
        l=make_unique<LogicalExpr>(move(l),TokenType::AND,move(r));
    }
    return l;
}

unique_ptr<Expr> Parser::logic_or(){
    unique_ptr<Expr> l=logic_and();
    while(match({TokenType::OR})){
        unique_ptr<Expr> r=logic_and();
        l=make_unique<LogicalExpr>(move(l),TokenType::OR,move(r));
    }
    return l;
}


unique_ptr<Expr> Parser::assignment(){
    unique_ptr<Expr> l=logic_or();
    if(match({TokenType::EQUAL})){
        if(VarExpr* vp=dynamic_cast<VarExpr*>(l.get())){
            Token n=vp->name;
            unique_ptr<Expr> value=assignment();
            return make_unique<AssignExpr>(move(value),n);
        }
        else{
            // todo error not valid
            cout<<"Error(assignment) line: "<<peek().line<<"\n";
        }
    }
    return l;
}


unique_ptr<Expr> Parser::expression(){
    return assignment();
}



//statements

unique_ptr<Stmt> Parser::print_statement(){
    unique_ptr<Expr> exp=expression();
    consume(TokenType::SEMICOLON, "print stat error"); // todo write error message

    return make_unique<PrintStmt>(move(exp)); //do i need move here ?
}


unique_ptr<Stmt> Parser::expression_statement(){
    unique_ptr<Expr> exp=expression();
    consume(TokenType::SEMICOLON, "expected semicolon error"); // todo write error message

    return make_unique<ExprStmt>(move(exp)); //do i need move here ?

}



unique_ptr<Stmt> Parser::var_declaration(){
    Token n=consume(TokenType::IDENTIFIER, "expected identifier error"); // todo write error message
    if(match({TokenType::EQUAL})){
        unique_ptr<Expr> ini=expression();
            consume(TokenType::SEMICOLON, "expected semicolon error"); // todo write error message
        
            return make_unique<VarDeclStmt>(n,move(ini)); //do i need move here ?
    }
    else{
        cout<<"Error(var_declaration) line: "<<peek().line<<"\n";
        return nullptr;     //todo error
    }
}



vector<unique_ptr<Stmt>> Parser::block() {
    vector<unique_ptr<Stmt>> statements;

    while (!check(TokenType::RIGHT_PAREN_CURLY) &&!is_at_end()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_PAREN_CURLY, "Expected '}' after block.");
    return statements;
}


unique_ptr<Stmt> Parser::if_statement() {
    consume(TokenType::LEFT_PAREN_CURVE, "Expected '(' after 'if'.");
    unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN_CURVE, "Expected ')' after if condition.");

    unique_ptr<Stmt> then_branch = statement();
    unique_ptr<Stmt> else_branch = nullptr;

    if (match({TokenType::ELSE})) {
        else_branch = statement();
    }

    return make_unique<IfStmt>(move(condition), move(then_branch), move(else_branch));
}

unique_ptr<Stmt> Parser::while_statement() {
    consume(TokenType::LEFT_PAREN_CURVE, "Expected '(' after 'while'.");
    unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN_CURVE, "Expected ')' after while condition.");
    
    unique_ptr<Stmt> body = statement();

    return make_unique<WhileStmt>(move(condition), move(body));
}


unique_ptr<Stmt> Parser::fun_declaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected function name.");
    consume(TokenType::LEFT_PAREN_CURVE, "Expected '(' after function name.");
    
    vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN_CURVE)) {
        do {
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name."));
        } while (match({TokenType::COMMA})); 
    }
    
    consume(TokenType::RIGHT_PAREN_CURVE, "Expected ')' after parameters.");
    consume(TokenType::LEFT_PAREN_CURLY, "Expected '{' before function body.");
    
    vector<unique_ptr<Stmt>> body = block();
    
    return make_unique<FunDeclStmt>(name, parameters, move(body));
}



unique_ptr<Stmt> Parser::return_statement() {
    Token keyword = previous(); 
    unique_ptr<Expr> value = nullptr;
    
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expected ';' after return value.");
    return make_unique<ReturnStmt>(keyword, move(value));
}


unique_ptr<Stmt> Parser::statement(){
    if(match({TokenType::PRINT})){
        return print_statement();
    }
    if(match({TokenType::IF})){
        return if_statement();
    }
    if(match({TokenType::WHILE})){
        return while_statement();
    }
    // if(match({TokenType::FOR})){
    //     return for_statement();
    // }
    if(match({TokenType::RETURN})){
        return return_statement();
    }
    if(match({TokenType::LEFT_PAREN_CURLY})){
        return make_unique<BlockStmt>(block()); // why no suitable user-defined conversion from "std::__detail::__unique_ptr_t<BlockStmt>" (aka "std::unique_ptr<BlockStmt, std::default_delete<BlockStmt>>") to "std::unique_ptr<Stmt, std::default_delete<Stmt>>" existsC/C++(312)
    }
    else return expression_statement();
}



unique_ptr<Stmt> Parser::declaration(){
    if(match({TokenType::DEF})){
        return fun_declaration();
    }
    if(match({TokenType::LET})){
        return var_declaration();
    }
    else{
        return statement();
    }
}

vector<unique_ptr<Stmt>> Parser::parse(){
    vector<unique_ptr<Stmt>> st;
    while(!is_at_end()){
        st.push_back(declaration());
    }
    return st;
}


