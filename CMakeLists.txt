cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

include_directories(E9Lang/include)

set(PARSER_SOURCE_FILES
        E9Lang/lang/parser/lexer.cpp E9Lang/lang/parser/lexer.hpp
        E9Lang/lang/parser/tokens.hpp
        E9Lang/lang/parser/util.cpp E9Lang/lang/parser/util.hpp
        E9Lang/lang/parser/tokentypes.hpp
        )
add_library(e9parser SHARED ${PARSER_SOURCE_FILES})

add_executable(lexer_test tests/lexer_test.cpp)
target_link_libraries(lexer_test e9parser)
