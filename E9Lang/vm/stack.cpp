//
// Created by egor9814 on 30.03.18.
//

#include "stack.hpp"

e9lang::vm::Stack::Stack() {
    env = new Environment;
}

e9lang::vm::Stack::~Stack() {
    env->clear();
    delete env;
}

void e9lang::vm::Stack::push() {
    stack.push(std::stack<Object *>());
}

void e9lang::vm::Stack::pop() {
    stack.pop();
}

void e9lang::vm::Stack::saveTop() {
    objects.push(stack.top().top());
}

void e9lang::vm::Stack::popTop() {
    stack.top().pop();
}

e9lang::vm::Object *e9lang::vm::Stack::top() {
    return stack.top().top();
}

void e9lang::vm::Stack::push(e9lang::vm::Object *o) {
    stack.top().push(o);
}

e9lang::vm::Environment *e9lang::vm::Stack::getEnv() {
    return env;
}
