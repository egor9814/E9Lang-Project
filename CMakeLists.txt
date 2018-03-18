cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

include_directories(E9Lang/include)

set(PARSER_SOURCE_FILES
        E9Lang/lang/parser/tokentypes.hpp
        E9Lang/lang/parser/tokens.cpp E9Lang/lang/parser/tokens.hpp
        E9Lang/lang/parser/lexer.cpp E9Lang/lang/parser/lexer.hpp
        E9Lang/lang/parser/util.cpp E9Lang/lang/parser/util.hpp
        E9Lang/lang/parser/parser.cpp E9Lang/lang/parser/parser.hpp
        )
set(AST_SOURCE_FILES
        E9Lang/lang/ast/ast.hpp
        E9Lang/lang/ast/unary.cpp
        E9Lang/lang/ast/binary.cpp
        E9Lang/lang/ast/value.cpp
        E9Lang/lang/ast/var.cpp
        E9Lang/lang/ast/if.cpp
        E9Lang/lang/ast/var_def.cpp
        E9Lang/lang/ast/const_def.cpp
        E9Lang/lang/ast/print.cpp
        E9Lang/lang/ast/fun.cpp
        E9Lang/lang/ast/block.cpp
        E9Lang/lang/ast/assign.cpp
        E9Lang/lang/ast/function_call.cpp
        E9Lang/lang/ast/ternary.cpp
        E9Lang/lang/ast/elvis.cpp
        E9Lang/lang/ast/container_access.cpp)
add_library(e9parser SHARED ${PARSER_SOURCE_FILES} ${AST_SOURCE_FILES})

add_executable(lexer_test tests/lexer_test.cpp tests/tester.hpp)
target_link_libraries(lexer_test e9parser)
