//
// Created by egor9814 on 14.03.18.
//

#ifndef E9LANG_PROJECT_UTIL_HPP
#define E9LANG_PROJECT_UTIL_HPP

#include <list>

namespace e9lang {

    namespace util {

        template<typename T>
        void deallocate(std::list<T *> &list);

    }

}

template<typename T>
void ::e9lang::util::deallocate(std::list<T *> &list) {
    while (!list.empty()) {
        auto pointer = list.front();
        list.pop_front();
        delete pointer;
    }
}

#endif //E9LANG_PROJECT_UTIL_HPP
