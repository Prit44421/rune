#include <string>
using namespace std;


enum class TokenType {
    LEFT_PAREN_CURVE, RIGHT_PAREN_CURVE, LEFT_PAREN_SQUARE, RIGHT_PAREN_SQUARE, LEFT_PAREN_CURLY, RIGHT_PAREN_CURLY,
    
    PLUS, MINUS, STAR, SLASH, DOT, SEMICOLON, PERCENT,
    
    EQUAL, EQUAL_EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

    IF, ELSE, FOR, WHILE, NOT, PRINT, RETURN, TRUE, FALSE,

    NUMBER, STRING, IDENTIFIER,

    END_OF_FILE
};

struct Token{
    TokenType type;
    string text;
    int line; //for error reporting

    Token(TokenType t, string s, int l){
        type=t;
        text=s;
        line=l;
    }

};

