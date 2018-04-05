//
// Created by egor9814 on 31.03.18.
//

#include "objects.hpp"
#include "math.hpp"

e9lang::objects::ObjectType e9lang::objects::Char::getType() {
    return ObjectType_CHAR;
}


e9lang::objects::Char *e9lang::objects::Char::toChar(e9lang::vm::Stack *s) {
    return this;
}

e9lang::objects::Int *e9lang::objects::Char::toInt(e9lang::vm::Stack *s) {
    if (s)
        return s->getEnv()->allocate<Int>((int) value);
    return nullptr;
}

e9lang::objects::Float *e9lang::objects::Char::toFloat(e9lang::vm::Stack *s) {
    if (s)
        return s->getEnv()->allocate<Float>((float) value);
    return nullptr;
}


template<typename T>
e9lang::objects::Char *asChar(e9lang::vm::Stack *s, T value) {
    return s->getEnv()->allocate<e9lang::objects::Char>((char) value);
}

e9lang::objects::Object *e9lang::objects::Char::operator_add(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    switch (other->getType()) {
        default:
            return nullptr;
        case ObjectType_CHAR:
            auto oc = other->toChar(s);
            if (oc->value == 0)
                return this;
            return asChar(s, value + oc->value);
        case ObjectType_INT:
            auto oi = other->toInt(s);
            if (oi->value == 0)
                return this;
            return asChar(s, value + oi->value);
        case ObjectType_FLOAT:
            auto of = other->toFloat(s);
            if (of->value == 0)
                return this;
            return asChar(s, value + of->value);
        case ObjectType_BOOL:
            return other->toRawBool() ? asChar(s, value + 1) : this;
        case ObjectType_STRING:
            return s->getEnv()->allocate<String>(toRawString() + other->toRawString());
    }
}

e9lang::objects::Object *e9lang::objects::Char::operator_sub(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    switch (other->getType()) {
        default:
            return nullptr;
        case ObjectType_CHAR:
            auto oc = other->toChar(s);
            if (oc->value == 0)
                return this;
            return asChar(s, value - oc->value);
        case ObjectType_INT:
            auto oi = other->toInt(s);
            if (oi->value == 0)
                return this;
            return asChar(s, value - oi->value);
        case ObjectType_FLOAT:
            auto of = other->toFloat(s);
            if (of->value == 0)
                return this;
            return asChar(s, value - of->value);
        case ObjectType_BOOL:
            return other->toRawBool() ? asChar(s, value - 1) : this;
    }
}

e9lang::objects::Object *e9lang::objects::Char::operator_mul(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    switch (other->getType()) {
        default:
            return nullptr;
        case ObjectType_CHAR:
            auto oc = other->toChar(s);
            if (oc->value == 0)
                return Zero;
            return asChar(s, value * oc->value);
        case ObjectType_INT:
            auto oi = other->toInt(s);
            if (oi->value == 0)
                return Zero;
            return asChar(s, value * oi->value);
        case ObjectType_FLOAT:
            auto of = other->toFloat(s);
            if (of->value == 0)
                return Zero;
            return asChar(s, value * of->value);
        case ObjectType_BOOL:
            return other->toRawBool() ? asChar(s, value * 1) : Zero;
        case ObjectType_STRING:
            return other->toString(s)->operator_mul(s, this);
    }
}

e9lang::objects::Object *e9lang::objects::Char::operator_div(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    switch (other->getType()) {
        default:
            return nullptr;
        case ObjectType_CHAR:
            auto oc = other->toChar(s);
            if (oc->value == 0)
                return value < 0 ? Float::NegativeInfinity : Float::Infinity;
            return asChar(s, value / oc->value);
        case ObjectType_INT:
            auto oi = other->toInt(s);
            if (oi->value == 0)
                return value < 0 ? Float::NegativeInfinity : Float::Infinity;
            return asChar(s, value / oi->value);
        case ObjectType_FLOAT:
            auto of = other->toFloat(s);
            if (of->value == 0)
                return value < 0 ? Float::NegativeInfinity : Float::Infinity;
            return asChar(s, value / of->value);
    }
}

e9lang::objects::Object *e9lang::objects::Char::operator_pow(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    switch (other->getType()) {
        default:
            return nullptr;
        case ObjectType_CHAR:
            auto oc = other->toChar(s);
            if (oc->value == 0)
                return this;
            return asChar(s, math::pow(value, oc->value));
        case ObjectType_INT:
            auto oi = other->toInt(s);
            if (oi->value == 0)
                return this;
            return asChar(s, math::pow(value, oi->value));
        case ObjectType_FLOAT:
            auto of = other->toFloat(s);
            if (of->value == 0)
                return this;
            return asChar(s, math::pow(value, of->value));
        case ObjectType_BOOL:
            return other->toRawBool() ? this : One;
    }
}

e9lang::objects::Object *e9lang::objects::Char::operator_rem(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    switch (other->getType()) {
        default:
            return nullptr;
        case ObjectType_CHAR:
            auto oc = other->toChar(s);
            if (oc->value == 0)
                return value < 0 ? Float::NegativeInfinity : Float::Infinity;
            return asChar(s, value % oc->value);
        case ObjectType_INT:
            auto oi = other->toInt(s);
            if (oi->value == 0)
                return value < 0 ? Float::NegativeInfinity : Float::Infinity;
            return asChar(s, value % oi->value);
    }
}
