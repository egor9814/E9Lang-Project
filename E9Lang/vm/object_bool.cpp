//
// Created by egor9814 on 31.03.18.
//

#include "objects.hpp"

e9lang::objects::Boolean::Boolean(bool value) : Object(), value(value) {
    initOperators();
}

unsigned long e9lang::objects::Boolean::hashCode() {
    return value ? 1 : 0;
}

e9lang::objects::ObjectType e9lang::objects::Boolean::getType() {
    return ObjectType_BOOL;
}

std::string e9lang::objects::Boolean::toRawString() {
    return value ? "true" : "false";
}

bool e9lang::objects::Boolean::toRawBool() {
    return value;
}

e9lang::objects::Object *e9lang::objects::Boolean::operator_not(e9lang::vm::Stack *s) {
    return value ? False : True;
}
