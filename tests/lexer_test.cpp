//
// Created by egor9814 on 14.03.18.
//

#include <iostream>
#include "../E9Lang/lang/parser/lexer.hpp"

#define USE_TEST(number) "../tests/test" #number ".enl"
int main(){
    using namespace e9lang::parser;
    FileSourceProvider sourceProvider(USE_TEST(1));
    if(!sourceProvider.open()){
        std::cerr << "cannot open input file" << std::endl;
        return -1;
    }
    LexerConfig config(4, false);
    Lexer lexer(config, sourceProvider);
    sourceProvider.close();

    if(lexer.tokenize()){
        for(auto &i : lexer.getTokens()) {
            std::cout << i->toString() << std::endl;
        }
    } else {
        for(auto &i : lexer.getWarnings()) {
            std::cout << i->toString() << std::endl;
        }
        for(auto &i : lexer.getErrors()) {
            std::cerr << i->toString() << std::endl;
        }
    }

    return 0;
}