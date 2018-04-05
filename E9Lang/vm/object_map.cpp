//
// Created by egor9814 on 31.03.18.
//

#include "objects.hpp"

e9lang::objects::Map::Map(unsigned long primeNumber) : Object(), prime(primeNumber) {}

unsigned long e9lang::objects::Map::hashCode() {
    auto result = prime;
    for(auto &i : vars2){
        result = prime * result + i.first;
        result = prime * result + i.second.second->hashCode();
    }
    return result;
}

e9lang::objects::ObjectType e9lang::objects::Map::getType() {
    return ObjectType_MAP;
}

void e9lang::objects::Map::init(e9lang::vm::Stack *s) {
    addConst("size", size(s));
}

void e9lang::objects::Map::put(e9lang::vm::Stack *s, e9lang::objects::Object *key, e9lang::objects::Object *value) {
    addVar(key, value);
    addConst("size", size(s));
}

e9lang::objects::ULong *e9lang::objects::Map::size(e9lang::vm::Stack *s) {
    auto size = vars2.size();
    if(size == 0)
        return ULong::Zero;
    if(size == 1)
        return ULong::One;
    return s->getEnv()->allocate<ULong>(size);
}
