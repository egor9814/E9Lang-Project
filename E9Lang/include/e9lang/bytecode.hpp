//
// Created by egor9814 on 27.03.18.
//

#ifndef E9LANG_PROJECT_BYTECODE_HPP
#define E9LANG_PROJECT_BYTECODE_HPP

#include <string>

namespace e9lang {

    namespace compiler {

        enum ByteCode {
            ByteCode_NOP,

            ByteCode_PUSH,
            ByteCode_POP,
            ByteCode_POP_TOP,

            ByteCode_STORE,
            ByteCode_STORE_VAR,
            ByteCode_DEFINE_CONST,
            ByteCode_LOAD_VAR,
            ByteCode_LOAD_CONST,

            ByteCode_UNARY_NEGATE,
            ByteCode_UNARY_NOT,
            ByteCode_UNARY_INVERT,

            ByteCode_BINARY_AND,
            ByteCode_BINARY_OR,
            ByteCode_BINARY_XOR,
            ByteCode_BINARY_SHIFT_RIGHT,
            ByteCode_BINARY_SHIFT_RIGHT_UNSIGNED,
            ByteCode_BINARY_SHIFT_LEFT,
            ByteCode_BINARY_SHIFT_LEFT_UNSIGNED,

            ByteCode_AND,
            ByteCode_OR,

            ByteCode_EQUALS,
            ByteCode_NOT_EQUALS,
            ByteCode_GT,
            ByteCode_GT_EQUALS,
            ByteCode_LT,
            ByteCode_LT_EQUALS,

            ByteCode_SUBSCR,
            ByteCode_CALL,

            ByteCode_ADD,
            ByteCode_SUB,
            ByteCode_MUL,
            ByteCode_DIV,
            ByteCode_REM,
            ByteCode_POW,

            ByteCode_INC,
            ByteCode_DEC,

            ByteCode_MAP,
            ByteCode_ARRAY,

            ByteCode_JMP,
            ByteCode_JMP_IF,

            ByteCode_LABEL,

            ByteCode_FUNCTION,
            ByteCode_FUNCTION_END,
            ByteCode_FUNCTION_WITHOUT_NAME,
            ByteCode_ARGUMENTS,
            ByteCode_ARGUMENT,
            ByteCode_ARGUMENT_VARARG,
            ByteCode_RETURN,

            ByteCode_PRINT,
        };

    }

}

#endif //E9LANG_PROJECT_BYTECODE_HPP
