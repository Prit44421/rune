#pragma once


#include <unordered_map>
#include <string>
#include "ast.h"

using namespace std;


class Environment{
private:
    unordered_map<string, Value> environment;
    shared_ptr<Environment> enclosing=nullptr;
    
public:
    void define(string n, Value v);
    Value get(Token n);
    void assign(Token n, Value v);

    Environment(){
        enclosing=nullptr;
    }
    Environment(shared_ptr<Environment> e){
        enclosing=e;
    }
};