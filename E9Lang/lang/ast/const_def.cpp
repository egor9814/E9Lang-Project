//
// Created by egor9814 on 15.03.18.
//

#include "../../include/e9lang/ast.hpp"

e9lang::ast::ConstStatement::ConstStatement(std::list<Token *> &names, std::list<Expression *> &values)
        : names(std::move(names)), values(std::move(values)) {}

std::string e9lang::ast::ConstStatement::toString() {
    std::stringstream ss;
    ss << "const ";
    auto name = names.begin(), name_end = names.end();
    auto value = values.begin();
    for (; name != name_end; value++) {
        ss << dynamic_cast<e9lang::parser::Token *>(*name)->text;
        ss << '=';
        auto v = *value;
        ss << (v ? v->toString() : "null");
        name++;
        if (name != name_end)
            ss << ", ";
    }
    return ss.str();
}

void e9lang::ast::ConstStatement::finalize() {
    while (!values.empty()) {
        auto v = values.front();
        values.pop_front();
        if (v)
            v->finalize();
    }
    delete this;
}

void e9lang::ast::ConstStatement::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}



