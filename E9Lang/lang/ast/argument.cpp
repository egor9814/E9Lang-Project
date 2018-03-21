//
// Created by egor9814 on 19.03.18.
//

#include "ast.hpp"

e9lang::ast::Argument::Argument(Token *name, bool var_arg)
        : name(name->text), var_arg(var_arg) {}

std::string &e9lang::ast::Argument::getName() {
    return name;
}

bool e9lang::ast::Argument::isVarArg() const {
    return var_arg;
}

std::string e9lang::ast::Argument::toString() {
    std::stringstream ss;
    ss << name;
    if(var_arg)
        ss << "[]";
    return ss.str();
}

void e9lang::ast::Argument::finalize() {
    delete this;
}
