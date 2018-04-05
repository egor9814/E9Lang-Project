//
// Created by egor9814 on 30.03.18.
//

#ifndef E9LANG_PROJECT_STACK_HPP
#define E9LANG_PROJECT_STACK_HPP

#include <stack>
#include "env.hpp"

namespace e9lang {

    namespace vm {

        class Object;

        class Stack {
            std::stack<std::stack<Object *>> stack;
            std::stack<Object *> objects;

            Environment* env;

        public:
            Stack();
            ~Stack();

            void push();

            void pop();


            void saveTop();

            void popTop();

            Object *top();

            void push(Object *);


            Environment* getEnv();
        };

    }

}

#endif //E9LANG_PROJECT_STACK_HPP
