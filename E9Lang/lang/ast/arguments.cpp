//
// Created by egor9814 on 19.03.18.
//

#include "ast.hpp"

void e9lang::ast::Arguments::add(Argument *arg) {
    args.push_back(arg);
}

bool e9lang::ast::Arguments::hasVarArgs() {
    return !args.empty() && args.back()->isVarArg();
}

std::string e9lang::ast::Arguments::toString() {
    std::stringstream ss;
    ss << "(";
    for(auto &i : args){
        ss << i->toString() << ", ";
    }
    ss << ")";
    return ss.str();
}

void e9lang::ast::Arguments::finalize() {
    while(!args.empty()){
        auto a = args.front();
        args.pop_front();
        a->finalize();
    }
    delete this;
}
