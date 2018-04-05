//
// Created by egor9814 on 31.03.18.
//

#include "objects.hpp"

e9lang::objects::Int::Int(int v) : Number(v) {}

e9lang::objects::ObjectType e9lang::objects::Int::getType() {
    return ObjectType_INT;
}
