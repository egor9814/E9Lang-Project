//
// Created by egor9814 on 16.03.18.
//

#include "../../include/e9lang/ast.hpp"

void e9lang::ast::BlockStatement::add(Statement *statement) {
    statements.push_back(statement);
}

std::string e9lang::ast::BlockStatement::toString() {
    if (statements.empty())
        return "{}";
    std::stringstream ss;
    ss << '{';
    /*auto end = statements.end();
    for(auto i = statements.begin(); i != end; ){
        ss << std::endl;
        ss << (*i)->toString();
    }*/
    for (auto &i : statements) {
        ss << std::endl << i->toString();
    }
    ss << std::endl << '}';
    return ss.str();
}

void e9lang::ast::BlockStatement::finalize() {
    while(!statements.empty()){
        auto s = statements.front();
        statements.pop_front();
        s->finalize();
    }
    delete this;
}

void e9lang::ast::BlockStatement::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}

