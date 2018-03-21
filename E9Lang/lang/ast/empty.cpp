//
// Created by egor9814 on 21.03.18.
//

#include "ast.hpp"

e9lang::ast::EmptyStatement::EmptyStatement() {}

std::string e9lang::ast::EmptyStatement::toString() {
    return ";";
}

void e9lang::ast::EmptyStatement::finalize() {
    delete this;
}
