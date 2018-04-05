//
// Created by egor9814 on 30.03.18.
//

#ifndef E9LANG_PROJECT_ENV_HPP
#define E9LANG_PROJECT_ENV_HPP

#include <list>
#include "../lang/parser/util.hpp"

namespace e9lang {

    namespace vm {

        class Environment {
            std::list<void *> objects;

        public:
            template<typename T, typename... Args>
            T *allocate(Args &&... args) {
                auto object = new T(args...);
                objects.push_back((void *) object);
                return object;
            }

            template<typename T>
            void deallocate(T *object) {
                objects.remove((void *) object);
                delete object;
            }

            void clear() {
                e9lang::util::deallocate(objects);
            }


            template<typename T>
            Environment &operator>>(T *&objectPointer) {
                objectPointer = allocate();
                return *this;
            }


            unsigned long getArrayPrime();

            unsigned long getMapPrime();

        };

    }

}

#endif //E9LANG_PROJECT_ENV_HPP
