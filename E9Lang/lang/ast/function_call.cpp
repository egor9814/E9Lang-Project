//
// Created by egor9814 on 17.03.18.
//

#include "ast.hpp"

e9lang::ast::FunctionCallExpression::FunctionCallExpression(Expression *name)
        : name(name) {}

void e9lang::ast::FunctionCallExpression::addArgument(e9lang::ast::Expression *arg) {
    args.push_back(arg);
}

std::string e9lang::ast::FunctionCallExpression::toString() {
    std::stringstream ss;
    ss << name->toString() << "(";
    for (auto &i : args) {
        ss << i->toString() << ", ";
    }
    ss << ")";
    return ss.str();
}

void e9lang::ast::FunctionCallExpression::finalize() {
    name->finalize();
    while (!args.empty()) {
        auto a = args.front();
        args.pop_front();
        a->finalize();
    }
    delete this;
}

