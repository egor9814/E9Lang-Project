//
// Created by egor9814 on 15.03.18.
//

#ifndef E9LANG_PROJECT_TESTER_HPP
#define E9LANG_PROJECT_TESTER_HPP

#define CREATE_TEST(TestName, FileName, Unit) int TestName() {\
const char* filePath = "../tests/" #FileName ".enl";\
Unit;\
return 0;\
}

#endif //E9LANG_PROJECT_TESTER_HPP
