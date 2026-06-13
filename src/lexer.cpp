#include "lexer.h"
// #include "tokens.h"

Lexer::Lexer(string s){
    source=s;
}

bool Lexer::is_at_end(){
    return current>=source.size();
}

char Lexer::advance(){
    if(!is_at_end()){
        return source[current++];
    }
    return '\0';
}

char Lexer::peek(){
    if(!is_at_end()) return source[current];
    return '\0';
}

char Lexer::peek_next(){
    current+=1;
    if(!Lexer::is_at_end()) return Lexer::source[current--];
    return '\0';
}

bool Lexer::match(char expected){
    if(is_at_end()) return false;
    if(peek()==expected) return true;
    return false;
}
    
void Lexer::add_token(TokenType t, string s, int l){
    Token to=Token(t,s,l);
    tokens.push_back(to);
    return;
}

void Lexer::handle_string(){
    string s="";
    while(peek()!='"'){
        s+=advance();
    }
    advance();
    add_token(TokenType::STRING,s,line);
    return ;
}

void Lexer::handle_number(){
    string s="";
    current--;
    while(isdigit(peek()) || peek()=='.'){
        s+=advance();
    }
    add_token(TokenType::NUMBER, s, line);
    return ;
}

void Lexer::handle_identifier(){
    string s="";
    current--;
    while(isalnum(peek())){
        s+=advance();
    }
    // switch(s){
    //     case "if":
    //         add_token(TokenType::IF, s, line);
    //     case "else":
    //         add_token(TokenType::ELSE, s, line);
    //     case "for":
    //         add_token(TokenType::FOR, s, line);
    //     case "while":
    //         add_token(TokenType::WHILE, s, line);
    //     // case "not":
    //     //     add_token(TokenType::NOT, s, line);
    //     case "print":
    //         add_token(TokenType::PRINT, s, line);
    //     case "return":
    //         add_token(TokenType::RETURN, s, line);
    //     case "TRUE":
    //         add_token(TokenType::TRUE, s, line);
    //     case "FALSE":
    //         add_token(TokenType::FALSE, s, line);
    //     default:
    //         add_token(TokenType::IDENTIFIER, s, line);
    // }
    if(s=="if"){
        add_token(TokenType::IF, s, line);
    }
    else if(s=="else"){
        add_token(TokenType::ELSE, s, line);
    }
    else if(s=="for"){
        add_token(TokenType::FOR, s, line);
    }
    else if(s=="while"){
        add_token(TokenType::WHILE, s, line);
    }
    // if(s=="not"){
    //     add_token(TokenType::NOT, s, line);
    // }
    else if(s=="print"){
        add_token(TokenType::PRINT, s, line);
    }
    else if(s=="return"){
        add_token(TokenType::RETURN, s, line);
    }
    else if(s=="TRUE"){
        add_token(TokenType::TRUE, s, line);
    }
    else if(s=="FALSE"){
        add_token(TokenType::FALSE, s, line);
    }
    else if(s=="let"){
        add_token(TokenType::LET, s, line);
    }
    else{
        add_token(TokenType::IDENTIFIER, s, line);
    }

}


vector<Token> Lexer::scan_tokens(){
    while(!is_at_end()){
        start=current;
        scan_token();
    }
    add_token(TokenType::END_OF_FILE, "EOF", line);
    return tokens;
}

void Lexer::scan_token(){
    char c=advance();
    switch(c){
        case '\n':
            line ++;
            break;
        case '"':
            handle_string();
            break;
        case '(':
            add_token(TokenType::LEFT_PAREN_CURVE, "(", line);
            break;
        case ')':
            add_token(TokenType::RIGHT_PAREN_CURVE, ")", line);
            break;
        case '[':
            add_token(TokenType::LEFT_PAREN_SQUARE, "[", line);
            break;
        case ']':
            add_token(TokenType::RIGHT_PAREN_SQUARE, "]", line);
        case '{':
            add_token(TokenType::LEFT_PAREN_CURLY, "{", line);
            break;
        case '}':
            add_token(TokenType::RIGHT_PAREN_CURLY, "}", line);
            break;
        case ';':
            add_token(TokenType::SEMICOLON, ";", line);
            break;
        case '=':
            if(match('=')){
                current++;
                add_token(TokenType::EQUAL_EQUAL,"==",line);
            }
            else{
                add_token(TokenType::EQUAL,"=",line);   
            }
            break;
        case '!':
            if(match('=')){
                current++;
                add_token(TokenType::NOT_EQUAL,"!=",line);
            }
            else{
                add_token(TokenType::NOT,"!",line);   
            }
            break;
        case '<':
            if(match('=')){
                current++;
                add_token(TokenType::LESS_EQUAL,"<=",line);
            }
            else{
                add_token(TokenType::LESS,"<",line);   
            }
            break;
        case '>':
            if(match('=')){
                current++;
                add_token(TokenType::GREATER_EQUAL,">=",line);
            }
            else{
                add_token(TokenType::GREATER,">",line);   
            }
            break;
        default:
            if(isdigit(c)){
                handle_number();
            }
            else if(isalpha(c)){
                handle_identifier();
            }
            // else
            break;
    }
}

// void 
