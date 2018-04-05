//
// Created by egor9814 on 31.03.18.
//

#include "objects.hpp"

e9lang::objects::Float::Float(float v) : Number(v) {}

e9lang::objects::ObjectType e9lang::objects::Float::getType() {
    return ObjectType_FLOAT;
}
