//
// Created by egor9814 on 21.03.18.
//

#include "ast.hpp"

e9lang::ast::WhileLoop::WhileLoop(bool postCondition, Expression *condition, Statement *body)
        : postCondition(postCondition), condition(condition), body(body) {}

std::string e9lang::ast::WhileLoop::toString() {
    std::stringstream ss;
    if(postCondition){
        ss << "do " << body->toString() << " while " << condition->toString();
    } else {
        ss << "while " << condition->toString() << " " << body->toString();
    }
    return ss.str();
}

void e9lang::ast::WhileLoop::finalize() {
    condition->finalize();
    body->finalize();
    delete this;
}
