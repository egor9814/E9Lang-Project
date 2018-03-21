//
// Created by egor9814 on 21.03.18.
//

#include "ast.hpp"

e9lang::ast::LoopControlStatement::LoopControlStatement(Token *type)
        : type(type) {}

std::string e9lang::ast::LoopControlStatement::toString() {
    return type->text;
}

void e9lang::ast::LoopControlStatement::finalize() {
    delete this;
}
