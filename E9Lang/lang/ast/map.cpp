//
// Created by egor9814 on 23.03.18.
//

#include "../../include/e9lang/ast.hpp"
#include "../parser/util.hpp"

e9lang::ast::MapExpression::MapExpression(std::list<Expression *> &keys, std::list<Expression *> &values)
        : keys(keys), values(values) {}

std::string e9lang::ast::MapExpression::toString() {
    std::stringstream ss;
    ss << '{';
    auto end = keys.end();
    auto value = values.begin();
    for(auto i = keys.begin(); i != end; value++){
        ss << '\n' << (*i)->toString() << " = " << (*value)->toString();
        i++;
        if(i != end)
            ss << ',';
    }
    if(!keys.empty())
        ss << '\n';
    ss << '}';
    return ss.str();
}

void e9lang::ast::MapExpression::finalize() {
    e9lang::util::finalize(keys);
    e9lang::util::finalize(values);
    delete this;
}

void e9lang::ast::MapExpression::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
