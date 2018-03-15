//
// Created by egor9814 on 15.03.18.
//

#include "ast.hpp"

e9lang::ast::ValueExpression::ValueExpression(Token *value)
        : value(value) {}

std::string e9lang::ast::ValueExpression::toString() {
    return value->text;
}

void e9lang::ast::ValueExpression::finalize() {
    delete this;
}
