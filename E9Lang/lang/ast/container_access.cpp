//
// Created by egor9814 on 18.03.18.
//

#include "ast.hpp"

e9lang::ast::ContainerAccessExpression::ContainerAccessExpression(std::list<Expression *> &indexes)
        : indexes(indexes) {}

std::string e9lang::ast::ContainerAccessExpression::toString() {
    auto it = indexes.begin();
    auto end = indexes.end();
    std::stringstream ss;
    ss << *it;
    for(it++; it != end; it++){
        ss << "[\"" << *it << "\"]";
    }
    return ss.str();
}

void e9lang::ast::ContainerAccessExpression::finalize() {
    while(!indexes.empty()){
        auto i = indexes.front();
        indexes.pop_front();
        i->finalize();
    }
    delete this;
}
