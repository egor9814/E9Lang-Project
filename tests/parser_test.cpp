//
// Created by egor9814 on 21.03.18.
//

#include <iostream>
#include "../E9Lang/lang/parser/lexer.hpp"
#include "../E9Lang/lang/parser/parser.hpp"

int Test1(){
    using namespace e9lang::parser;
    FileSourceProvider sourceProvider("../tests/test1.enl");
    if(!sourceProvider.open()){
        std::cerr << "cannot open input file" << std::endl;
        return -1;
    }
    LexerConfig config;
    Lexer lexer(config, sourceProvider);
    sourceProvider.close();

    if(lexer.tokenize()){
        auto tokens = lexer.getTokens();
        /*for(auto &i : tokens) {
            std::cout << i->toString() << std::endl;
        }*/
        Parser parser(tokens);
        if(parser.parse()){
            auto unit = parser.getUnit();
            std::cout << unit->toString() << std::endl;
            return 0;
        } else {
            for(auto &i : parser.getErrors()){
                std::cerr << i->toString() << std::endl;
            }
        }
    } else {
        for(auto &i : lexer.getWarnings()) {
            std::cout << i->toString() << std::endl;
        }
        for(auto &i : lexer.getErrors()) {
            std::cerr << i->toString() << std::endl;
        }
    }
    return -1;
}

int main(){
    return Test1();
}