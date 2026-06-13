#include "environment.h"

void Environment::define(string n, Value v){
    if(environment.find(n)!=environment.end()){
        environment[n]=v;
    }
    else{
        environment[n]=v;
    }
}


Value Environment::get(Token n){
    if(environment.find(n.text)!=environment.end()){
        return environment[n.text];
    }
    else{
        if(enclosing!=nullptr){
            return enclosing->get(n);
        }
        else{
            //error
        }
    }
}



void Environment::assign(Token n, Value v){
    if(environment.find(n.text)!=environment.end()){
        environment[n.text]=v;
        return ;
    }
    else{
        if(enclosing!=nullptr){
            enclosing->get(n);
            return ;
        }
        else{
            //error
        }
    }
}