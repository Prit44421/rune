#include <iostream>
#include "lexer.h"
using namespace std;

int main(int argc, char* argv[]){


    
    if(argc<2){
        cout<<"ERROR: No file provided\nUsage: rune <script.rune>\n";
    }
    else{
        cout<<"File name: "<<argv[1]<<"\n";
    }

    

}