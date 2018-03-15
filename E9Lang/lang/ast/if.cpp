//
// Created by egor9814 on 15.03.18.
//

#include "ast.hpp"

e9lang::ast::IfStatement::IfStatement(Expression *condition, Statement *thenStatement, Statement *elseStatement)
        : condition(condition), thenStatement(thenStatement), elseStatement(elseStatement) {}

std::string e9lang::ast::IfStatement::toString() {
    std::stringstream ss;
    ss << "if " << condition->toString();
    ss << thenStatement->toString();
    if (elseStatement)
        ss << " else " << elseStatement->toString();
    return ss.str();
}

void e9lang::ast::IfStatement::finalize() {
    condition->finalize();
    thenStatement->finalize();
    if (elseStatement)
        elseStatement->finalize();
    delete this;
}
