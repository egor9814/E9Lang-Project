//
// Created by egor9814 on 14.03.18.
//

#include <iostream>
#include "e9lang/lexer.hpp"
#include "tester.hpp"

int lex(const char* filePath){
    using namespace e9lang::parser;
    FileSourceProvider sourceProvider(filePath);
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

CREATE_TEST(test_tokens, test0, {
    return lex(filePath);
})

CREATE_TEST(test_script, test1, {
    return lex(filePath);
})

int main(){
    return test_script();
}