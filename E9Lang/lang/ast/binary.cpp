//
// Created by egor9814 on 15.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::BinaryExpression::BinaryExpression(Expression *left, Expression *right, Token *operation)
        : left(left), right(right), operation(operation) {}

std::string e9lang::ast::BinaryExpression::toString() {
    return '(' + left->toString() + " " + operation->text + " " + right->toString() + ')';
}

void e9lang::ast::BinaryExpression::finalize() {
    left->finalize();
    right->finalize();
    delete this;
}

void e9lang::ast::BinaryExpression::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}


