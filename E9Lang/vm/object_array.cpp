//
// Created by egor9814 on 31.03.18.
//

#include "objects.hpp"

e9lang::objects::Array::Array(unsigned long prime) : Object(), prime(prime) {
    initOperators();
}

unsigned long e9lang::objects::Array::hashCode() {
    auto result = prime;
    for(auto &i : vars2){
        result = prime * result + i.second.second->hashCode();
    }
    return result;
}

e9lang::objects::ObjectType e9lang::objects::Array::getType() {
    return ObjectType_ARRAY;
}

void e9lang::objects::Array::init(e9lang::vm::Stack *s) {
    addConst("length", length(s));
}

void e9lang::objects::Array::add(e9lang::vm::Stack *s, e9lang::objects::Object *o) {
    addVar(length(s), o);
    addConst("length", length(s));
}

e9lang::objects::ULong *e9lang::objects::Array::length(e9lang::vm::Stack *s) {
    auto len = vars2.size();
    if(len == 0)
        return ULong::Zero;
    if(len == 1)
        return ULong::One;
    return s->getEnv()->allocate<ULong>(len);
}
