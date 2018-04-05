//
// Created by egor9814 on 21.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::EmptyStatement::EmptyStatement() {}

std::string e9lang::ast::EmptyStatement::toString() {
    return ";";
}

void e9lang::ast::EmptyStatement::finalize() {
    delete this;
}

void e9lang::ast::EmptyStatement::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
