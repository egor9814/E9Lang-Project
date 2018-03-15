//
// Created by egor9814 on 15.03.18.
//

#include "ast.hpp"

e9lang::ast::FunStatement::FunStatement(Token *functionName, std::list<Token *> &args, Statement *body)
        : name(functionName), args(std::move(args)), body(body) {}

std::string e9lang::ast::FunStatement::toString() {
    std::stringstream ss;
    ss << "fun " << name->text << '(';
    auto end = args.end();
    for (auto i = args.begin(); i != end;) {
        ss << dynamic_cast<e9lang::parser::Token *>(*i)->text;
        i++;
        if (i != end)
            ss << ", ";
    }
    ss << ") ";
    ss << body->toString();
    return ss.str();
}

void e9lang::ast::FunStatement::finalize() {
    body->finalize();
    delete this;
}
