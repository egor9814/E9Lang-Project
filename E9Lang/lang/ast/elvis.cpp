//
// Created by egor9814 on 18.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::ElvisExpression::ElvisExpression(Expression *condition, Expression *value)
        : condition(condition), value(value) {}

std::string e9lang::ast::ElvisExpression::toString() {
    return condition->toString() + " ?: " + value->toString();
}

void e9lang::ast::ElvisExpression::finalize() {
    condition->finalize();
    value->finalize();
    delete this;
}

void e9lang::ast::ElvisExpression::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
