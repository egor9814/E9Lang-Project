//
// Created by egor9814 on 27.03.18.
//

#include <e9lang/ast.hpp>

e9lang::ast::Unit::Unit(std::list<e9lang::ast::FunStatement *> &functions)
        : functions(functions) {}

std::string e9lang::ast::Unit::toString() {
    std::stringstream ss;
    for (auto &i : functions) {
        ss << "\n";
        ss << i->toString();
        ss << "\n";
    }
    return ss.str();
}

void e9lang::ast::Unit::finalize() {
    while (!functions.empty()) {
        auto f = functions.front();
        functions.pop_front();
        f->finalize();
    }
    delete this;
}

void e9lang::ast::Unit::accept(e9lang::ast::Visitor *visitor) {
    visitor->visit(this);
}