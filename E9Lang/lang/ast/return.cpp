//
// Created by egor9814 on 19.03.18.
//

#include "ast.hpp"

e9lang::ast::ReturnStatement::ReturnStatement(Expression *value)
        : value(value) {}

std::string e9lang::ast::ReturnStatement::toString() {
    return "return " + value->toString();
}

void e9lang::ast::ReturnStatement::finalize() {
    value->finalize();
    delete this;
}
