//
// Created by egor9814 on 21.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::LoopControlStatement::LoopControlStatement(Token *type)
        : type(type) {}

std::string e9lang::ast::LoopControlStatement::toString() {
    return type->text;
}

void e9lang::ast::LoopControlStatement::finalize() {
    delete this;
}

void e9lang::ast::LoopControlStatement::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}
