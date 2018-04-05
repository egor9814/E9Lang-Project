//
// Created by egor9814 on 15.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::PrintStatement::PrintStatement(Expression *message)
        : message(message) {}

std::string e9lang::ast::PrintStatement::toString() {
    return "print " + message->toString();
}

void e9lang::ast::PrintStatement::finalize() {
    message->finalize();
    delete this;
}

void e9lang::ast::PrintStatement::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}


