//
// Created by egor9814 on 15.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::ValueExpression::ValueExpression(Token *value)
        : value(value->text), type(value->type) {}

std::string e9lang::ast::ValueExpression::toString() {
    if(type == TokenType_STRING){
        return '"' + value + '"';
    } else if(type == TokenType_CHAR){
        return "'" + value + "'";
    } else {
        return value;
    }
}

void e9lang::ast::ValueExpression::finalize() {
    delete this;
}

void e9lang::ast::ValueExpression::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
