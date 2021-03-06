//
// Created by egor9814 on 18.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::TernaryExpression::TernaryExpression(Expression *condition, Expression *trueExpression,
                                                  Expression *falseExpression)
        : condition(condition), trueExpression(trueExpression), falseExpression(falseExpression){}

std::string e9lang::ast::TernaryExpression::toString() {
    return condition->toString() + " ? " + trueExpression->toString() + " : " + falseExpression->toString();
}

void e9lang::ast::TernaryExpression::finalize() {
    condition->finalize();
    trueExpression->finalize();
    falseExpression->finalize();
    delete this;
}

void e9lang::ast::TernaryExpression::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
