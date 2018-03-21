//
// Created by egor9814 on 17.03.18.
//

#include "ast.hpp"

e9lang::ast::AssignExpression::AssignExpression(AccessibleExpression *target, Expression *value, Token *operation)
        : target(target), value(value), operation(operation) {}

std::string e9lang::ast::AssignExpression::toString() {
    return target->toString() + " " + operation->text + " " + value->toString();
}

void e9lang::ast::AssignExpression::finalize() {
    target->finalize();
    value->finalize();
    delete this;
}
