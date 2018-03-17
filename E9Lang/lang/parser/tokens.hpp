//
// Created by egor9814 on 13.03.18.
//

#ifndef E9LANG_PROJECT_TOKENS_HPP
#define E9LANG_PROJECT_TOKENS_HPP

#include <string>
#include <sstream>
#include "tokentypes.hpp"

namespace e9lang {

    namespace parser {

        struct Range {
            unsigned long begin, end;
        };

        struct Location {
            unsigned long row, col;
        };

        struct Token {
            TokenType type;
            Range range;
            Location begin, end;
            std::string text;
            std::string errorText;

            Token(TokenType type, Range range, Location begin, Location end,
                  const std::string &text, const std::string &errorText = "");

            std::string getType();

            std::string toString();
        };
    }

}

#endif //E9LANG_PROJECT_TOKENS_HPP
