cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

include_directories(E9Lang/include)

set(PARSER_SOURCE_FILES
        E9Lang/include/e9lang/tokentypes.hpp
        E9Lang/lang/parser/tokens.cpp E9Lang/include/e9lang/tokens.hpp
        E9Lang/lang/parser/lexer.cpp E9Lang/include/e9lang/lexer.hpp
        E9Lang/lang/parser/util.cpp E9Lang/lang/parser/util.hpp
        E9Lang/lang/parser/parser.cpp E9Lang/include/e9lang/parser.hpp
        )
set(AST_SOURCE_FILES
        E9Lang/include/e9lang/ast.hpp
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
        E9Lang/lang/ast/container_access.cpp
        E9Lang/lang/ast/argument.cpp
        E9Lang/lang/ast/arguments.cpp
        E9Lang/lang/ast/return.cpp
        E9Lang/lang/ast/loop_ctrl.cpp
        E9Lang/lang/ast/while.cpp
        E9Lang/lang/ast/for.cpp
        E9Lang/lang/ast/empty.cpp
        E9Lang/lang/ast/expr.cpp
        E9Lang/lang/ast/array.cpp
        E9Lang/lang/ast/map.cpp
        E9Lang/lang/ast/node.cpp
        #E9Lang/include/e9lang/visitor.hpp
        E9Lang/lang/visitors/abs_vis.cpp
        E9Lang/lang/visitors/abs_vis.hpp
        E9Lang/lang/ast/unit.cpp
        )
add_library(e9parser SHARED ${PARSER_SOURCE_FILES} ${AST_SOURCE_FILES})

set(COMPILER_SOURCE_FILES
        E9Lang/include/e9lang/compiler.hpp
        E9Lang/lang/compiler/compiler.cpp
        E9Lang/include/e9lang/bytecode.hpp
        )
add_library(e9compiler SHARED ${COMPILER_SOURCE_FILES})
target_link_libraries(e9compiler e9parser)

set(VM_SOURCE_FILES
        E9Lang/vm/vm.cpp
        E9Lang/vm/vm.hpp
        E9Lang/vm/objects.cpp
        E9Lang/vm/objects.hpp
        E9Lang/vm/stack.cpp
        E9Lang/vm/stack.hpp
        E9Lang/vm/env.cpp
        E9Lang/vm/env.hpp
        E9Lang/vm/object_char.cpp
        E9Lang/vm/object_bool.cpp
        E9Lang/vm/object_null.cpp
#        E9Lang/vm/object_uchar.cpp
#        E9Lang/vm/object_short.cpp
#        E9Lang/vm/object_ushort.cpp
        E9Lang/vm/object_int.cpp
#        E9Lang/vm/object_uint.cpp
#        E9Lang/vm/object_long.cpp
#        E9Lang/vm/object_ulong.cpp
        E9Lang/vm/object_float.cpp
#        E9Lang/vm/object_double.cpp
        E9Lang/vm/object_array.cpp
        E9Lang/vm/object_map.cpp
        E9Lang/vm/object_string.cpp
        E9Lang/vm/object_fun.cpp
        E9Lang/vm/math.cpp
        E9Lang/vm/math.hpp
        E9Lang/vm/big_int.cpp
        E9Lang/vm/mutable_big_int.cpp
        )
add_executable(e9 ${VM_SOURCE_FILES})

add_executable(lexer_test tests/lexer_test.cpp tests/tester.hpp)
target_link_libraries(lexer_test e9parser)

add_executable(parser_test tests/parser_test.cpp tests/tester.hpp)
target_link_libraries(parser_test e9parser)

add_executable(compiler_test tests/compiler_test.cpp)
target_link_libraries(compiler_test e9compiler)