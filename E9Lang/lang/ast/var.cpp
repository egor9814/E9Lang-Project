//
// Created by egor9814 on 15.03.18.
//

#include "ast.hpp"

e9lang::ast::VarExpression::VarExpression(Token *varName)
        : name(varName->text) {}

std::string e9lang::ast::VarExpression::toString() {
    return name;
}

void e9lang::ast::VarExpression::finalize() {
    delete this;
}
