//
// Created by egor9814 on 15.03.18.
//

#include "ast.hpp"

e9lang::ast::VarStatement::VarStatement(std::list<Token *> &names, std::list<Token *> &values)
        : names(std::move(names)), values(std::move(values)) {}

std::string e9lang::ast::VarStatement::toString() {
    std::stringstream ss;
    ss << "var ";
    auto name = names.begin(), name_end = names.end();
    auto value = names.begin();
    for (; name != name_end; value++) {
        ss << dynamic_cast<e9lang::parser::Token *>(*name)->text;
        ss << '=';
        ss << dynamic_cast<e9lang::parser::Token *>(*value)->text;
        name++;
        if (name != name_end)
            ss << ", ";
    }
    return ss.str();
}

void e9lang::ast::VarStatement::finalize() {
    delete this;
}



