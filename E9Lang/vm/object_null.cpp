//
// Created by egor9814 on 31.03.18.
//

#include "objects.hpp"

e9lang::objects::Null::Null() : Object() {
    addOperator("not", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        return operator_not(s);
    });
    addOperator("eq", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_eq(s, args[0]);
    });
    addOperator("noteq", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_noteq(s, args[0]);
    });
}

unsigned long e9lang::objects::Null::hashCode() {
    return 0;
}

e9lang::objects::ObjectType e9lang::objects::Null::getType() {
    return ObjectType_NULL;
}

std::string e9lang::objects::Null::toRawString() {
    return "null";
}

bool e9lang::objects::Null::toRawBool() {
    return false;
}

e9lang::objects::Null *e9lang::objects::Null::toNull(vm::Stack *s) {
    return this;
}

e9lang::objects::Object *e9lang::objects::Null::operator_not(e9lang::vm::Stack *s) {
    return Boolean::True;
}

e9lang::objects::Object *e9lang::objects::Null::operator_eq(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    return other->getType() == ObjectType_NULL ? Boolean::True : Boolean::False;
}

e9lang::objects::Object *e9lang::objects::Null::operator_noteq(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    return other->getType() != ObjectType_NULL ? Boolean::True : Boolean::False;
}
