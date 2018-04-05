//
// Created by egor9814 on 23.03.18.
//

#include "../../include/e9lang/ast.hpp"
#include "../parser/util.hpp"

e9lang::ast::ArrayExpression::ArrayExpression(std::list<Expression *> &values)
        : values(values) {}

std::string e9lang::ast::ArrayExpression::toString() {
    std::stringstream ss;
    ss << '[';
    auto end = values.end();
    for(auto i = values.begin(); i != end; ){
        ss << (*i)->toString();
        i++;
        if(i != end)
            ss << ", ";
    }
    ss << ']';
    return ss.str();
}

void e9lang::ast::ArrayExpression::finalize() {
    e9lang::util::finalize(values);
    delete this;
}

void e9lang::ast::ArrayExpression::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
