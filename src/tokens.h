#pragma once

#include <string>
#include <iostream>
using namespace std;


enum class TokenType {
    LEFT_PAREN_CURVE, RIGHT_PAREN_CURVE, LEFT_PAREN_SQUARE, RIGHT_PAREN_SQUARE, LEFT_PAREN_CURLY, RIGHT_PAREN_CURLY,
    
    PLUS, MINUS, STAR, SLASH, DOT, SEMICOLON, PERCENT,
    
    EQUAL, EQUAL_EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    IF, ELSE, FOR, WHILE, NOT, PRINT, RETURN, TRUE, FALSE,

    NUMBER, STRING, IDENTIFIER,

    AND, OR, COMMA, DEF, LET,NIL,

    END_OF_FILE
};

inline const char* token_type_name(TokenType type) {
    switch (type) {
        case TokenType::LEFT_PAREN_CURVE:  return "LEFT_PAREN_CURVE";
        case TokenType::RIGHT_PAREN_CURVE: return "RIGHT_PAREN_CURVE";
        case TokenType::LEFT_PAREN_SQUARE: return "LEFT_PAREN_SQUARE";
        case TokenType::RIGHT_PAREN_SQUARE:return "RIGHT_PAREN_SQUARE";
        case TokenType::LEFT_PAREN_CURLY:  return "LEFT_PAREN_CURLY";
        case TokenType::RIGHT_PAREN_CURLY: return "RIGHT_PAREN_CURLY";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::DOT: return "DOT";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::PERCENT: return "PERCENT";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::FOR: return "FOR";
        case TokenType::WHILE: return "WHILE";
        case TokenType::NOT: return "NOT";
        case TokenType::PRINT: return "PRINT";
        case TokenType::RETURN: return "RETURN";
        case TokenType::TRUE: return "TRUE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::END_OF_FILE: return "EOF";
    }
    return "UNKNOWN";
}

struct Token{
    TokenType type;
    string text;
    int line; //for error reporting

    Token(TokenType t, string s, int l){
        type=t;
        text=s;
        line=l;
    }

    void print_token(){
        cout<<token_type_name(type)<<" :: "<<text<<" :: "<<line<<"\n";
    }

};
