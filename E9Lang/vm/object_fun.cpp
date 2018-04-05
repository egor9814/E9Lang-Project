//
// Created by egor9814 on 31.03.18.
//

#include "objects.hpp"

e9lang::objects::Function::Function(FunctionType &&function, bool isBuiltIn = false)
        : Object(), function(function), builtIn(isBuiltIn) {}

unsigned long e9lang::objects::Function::hashCode() {
    return 0;
}

e9lang::objects::ObjectType e9lang::objects::Function::getType() {
    return ObjectType_FUNCTION;
}

bool e9lang::objects::Function::isBuiltIn() const {
    return builtIn;
}

e9lang::objects::Object *e9lang::objects::Function::call(e9lang::vm::Stack *s, std::vector<Object *> &args) {
    return function(s, args);
}
