//
// Created by egor9814 on 18.03.18.
//

#include <vector>
#include "../../include/e9lang/ast.hpp"
#include "../parser/util.hpp"

e9lang::ast::ContainerAccessExpression::ContainerAccessExpression(std::vector<Expression *> &indexes)
        : indexes(indexes) {}

e9lang::ast::ContainerAccessExpression::ContainerAccessExpression(e9lang::ast::Expression *target,
                                                                  std::vector<Expression *> &indexes)
        : indexes(){
    this->indexes.push_back(target);
    for(auto &i : indexes)
        this->indexes.push_back(i);
}

e9lang::ast::ContainerAccessExpression::ContainerAccessExpression(e9lang::ast::Expression *target)
        : indexes() {
    indexes.push_back(target);
}

std::string e9lang::ast::ContainerAccessExpression::toString() {
    auto it = indexes.begin();
    auto end = indexes.end();
    std::stringstream ss;
    ss << (*it)->toString();
    for(it++; it != end; it++){
        ss << "[\"" << (*it)->toString() << "\"]";
    }
    return ss.str();
}

void e9lang::ast::ContainerAccessExpression::finalize() {
    /*while(!indexes.empty()){
        auto i = indexes.front();
        indexes.pop_front();
        i->finalize();
    }*/
    util::finalize(indexes);
    delete this;
}

void e9lang::ast::ContainerAccessExpression::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
