//
// Created by egor9814 on 14.03.18.
//

#ifndef E9LANG_PROJECT_UTIL_HPP
#define E9LANG_PROJECT_UTIL_HPP

#include <list>

namespace e9lang {

    namespace util {

        template<class Container>
        void deallocate(Container &list);


        template<class Container>
        void finalize(Container &list);
    }

}

template<class Container>
void ::e9lang::util::deallocate(Container &list) {
    while (!list.empty()) {
        auto pointer = list.back();
        list.pop_back();
        delete pointer;
    }
}


template<class Container>
void ::e9lang::util::finalize(Container &list) {
    while (!list.empty()) {
        auto pointer = list.back();
        list.pop_back();
        pointer->finalize();
    }
}

#endif //E9LANG_PROJECT_UTIL_HPP
