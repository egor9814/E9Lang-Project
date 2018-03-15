//
// Created by egor9814 on 15.03.18.
//

#include "ast.hpp"

e9lang::ast::UnaryExpression::UnaryExpression(Expression *target, Token *operation)
        : target(target), operation(operation) {}

std::string e9lang::ast::UnaryExpression::toString() {
    if (operation->type == TokenType_PLUS_PLUS || operation->type == TokenType_MINUS_MINUS) {
        return target->toString() + operation->text;
    } else {
        return operation->text + target->toString();
    }
}

void e9lang::ast::UnaryExpression::finalize() {
    target->finalize();
    delete this;
}

