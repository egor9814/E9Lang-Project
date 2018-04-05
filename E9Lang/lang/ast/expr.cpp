//
// Created by egor9814 on 21.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::ExpressionStatement::ExpressionStatement(Expression *expression)
        : expression(expression) {}

std::string e9lang::ast::ExpressionStatement::toString() {
    return expression->toString();
}

void e9lang::ast::ExpressionStatement::finalize() {
    expression->finalize();
    delete this;
}

void e9lang::ast::ExpressionStatement::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
