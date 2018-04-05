//
// Created by egor9814 on 21.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::ForLoop::ForLoop(Statement *init, Expression *condition, Statement *update, Statement *body)
        : init(init), condition(condition), update(update), body(body) {}

std::string e9lang::ast::ForLoop::toString() {
    std::stringstream ss;
    ss << "for ";
    if (init != nullptr)
        ss << init->toString();
    ss << "; ";
    if (condition != nullptr)
        ss << condition->toString();
    ss << "; ";
    if (update != nullptr)
        ss << update->toString();
    ss << " " << body->toString();
    return ss.str();
}

void e9lang::ast::ForLoop::finalize() {
    if (init)
        init->finalize();
    if (condition)
        condition->finalize();
    if (update)
        update->finalize();
    body->finalize();
    delete this;
}

void e9lang::ast::ForLoop::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
