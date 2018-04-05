//
// Created by egor9814 on 15.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::FunStatement::FunStatement(Token *functionName, Arguments* args, Statement *body)
        : name(functionName), args(args), body(body) {}

std::string e9lang::ast::FunStatement::toString() {
    std::stringstream ss;
    ss << "fun ";
    if(name)
        ss << name->text;
    ss << args->toString();
    ss << body->toString();
    return ss.str();
}

void e9lang::ast::FunStatement::finalize() {
    args->finalize();
    body->finalize();
    delete this;
}

void e9lang::ast::FunStatement::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
