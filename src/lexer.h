#include "tokens.h"
#include <string>
#include <vector>

using namespace std;



class Lexer{
public:
    Lexer(string s);

    vector<Token> scan_tokens();


private:    
    string source;
    vector<Token> tokens;
    int start=0;
    int current=0;
    int line=1;

    void scan_token();

    bool is_at_end();

    char advance();
    char peek();
    char peek_next();
    bool match(char expected);
        
    void add_token(TokenType t, string s, int l);

    void handle_string();
    void handle_number();
    void handle_identifier();

};