//
// Created by egor9814 on 31.03.18.
//

#include "objects.hpp"

e9lang::objects::String::String(const std::string &value) : Object(), value(value) {
    initOperators();
}

unsigned long e9lang::objects::String::hashCode() {
    return std::hash<std::string>()(value);
}

e9lang::objects::ObjectType e9lang::objects::String::getType() {
    return ObjectType_STRING;
}

void e9lang::objects::String::init(e9lang::vm::Stack *s) {
    addConst("length", length(s));
}

e9lang::objects::ULong *e9lang::objects::String::length(e9lang::vm::Stack *s) {
    auto len = value.size();
    if(len == 0)
        return ULong::Zero;
    if(len == 1)
        return ULong::One;
    return s->getEnv()->allocate<ULong>(len);
}
