//
// Created by egor9814 on 17.03.18.
//

#include "../../include/e9lang/ast.hpp"
#include "../parser/util.hpp"

e9lang::ast::FunctionCallExpression::FunctionCallExpression(Expression *name)
        : name(name) {}

void e9lang::ast::FunctionCallExpression::addArgument(e9lang::ast::Expression *arg) {
    args.push_back(arg);
}

std::string e9lang::ast::FunctionCallExpression::toString() {
    std::stringstream ss;
    ss << name->toString() << "(";
    auto end = args.end();
    for (auto i = args.begin(); i != end;) {
        ss << (*i)->toString();
        i++;
        if (i != end)
            ss << ", ";
    }
    ss << ")";
    return ss.str();
}

void e9lang::ast::FunctionCallExpression::finalize() {
    name->finalize();
    /*while (!args.empty()) {
        auto a = args.front();
        args.pop_front();
        a->finalize();
    }*/
    util::finalize(args);
    delete this;
}

void e9lang::ast::FunctionCallExpression::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}

