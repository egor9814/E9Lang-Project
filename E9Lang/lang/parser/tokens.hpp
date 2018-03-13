//
// Created by egor9814 on 13.03.18.
//

#ifndef E9LANG_PROJECT_TOKENS_HPP
#define E9LANG_PROJECT_TOKENS_HPP

#include <string>
#include <sstream>

namespace e9lang {

    namespace parser {

        enum TokenType {

            TokenType_EOF = 0, /// EOF

            TokenType_NULL, /// null reference
            TokenType_CHAR, TokenType_UCHAR, /// signed/unsigned char types
            TokenType_SHORT, TokenType_USHORT, /// signed/unsigned short types
            TokenType_INT, TokenType_UINT, /// signed/unsigned int types
            TokenType_LONG, TokenType_ULONG, /// signed/unsigned long types
            TokenType_FLOAT, TokenType_DOUBLE, /// float-point number types
            TokenType_BOOL, /// bool type
            TokenType_STRING, /// string type
            TokenType_ARRAY, TokenType_MAP,
            TokenType_IDENTIFIER, /// word

            TokenType_IF, TokenType_ELSE,
            TokenType_FOR, TokenType_WHILE, TokenType_DO,
            TokenType_CONTINUE, TokenType_BREAK, TokenType_RETURN,
            TokenType_FUN,
            TokenType_VAR, TokenType_CONST,
            TokenType_PRINT, /// temporary KOSTYL' for output info

            TokenType_DOT, /// .
            TokenType_COMMA, /// ,
            TokenType_COLON, /// :
            TokenType_SEMICOLON, /// ;
            TokenType_COLON_COLON, /// ::
            TokenType_LPAREN, TokenType_RPAREN, /// ()
            TokenType_LBRACKET, TokenType_RBRACKET, /// []
            TokenType_LBRACE, TokenType_RBRACE, /// {}

            TokenType_EQ, /// =
            TokenType_EQ_EQ, /// ==
            TokenType_EXCL, /// !
            TokenType_EXCL_EQ, /// !=
            TokenType_HOOK, /// ?
            TokenType_HOOK_EQ, /// ?=
            TokenType_LT, /// <
            TokenType_LT_EQ, /// <=
            TokenType_GT, /// >
            TokenType_GT_EQ, /// >=

            TokenType_SHIFT_LEFT, /// <<
            TokenType_SHIFT_LEFT_EQ, /// <<=
            TokenType_U_SHIFT_LEFT, /// <<<
            TokenType_U_SHIFT_LEFT_EQ, /// <<<=
            TokenType_SHIFT_RIGHT, /// >>
            TokenType_SHIFT_RIGHT_EQ, /// >>=
            TokenType_U_SHIFT_RIGHT, /// >>>
            TokenType_U_SHIFT_RIGHT_EQ, /// >>>=

            TokenType_PLUS, /// +
            TokenType_PLUS_PLUS, /// ++
            TokenType_PLUS_EQ, /// +=
            TokenType_MINUS, /// -
            TokenType_MINUS_MINUS, /// --
            TokenType_MINUS_EQ, /// -=
            TokenType_STAR, /// *
            TokenType_STAR_STAR, /// **
            TokenType_STAR_EQ, /// *=
            TokenType_STAR_STAR_EQ, /// **=
            TokenType_SLASH, /// /
            TokenType_SLASH_EQ, /// /=
            TokenType_PERCENT, /// %
            TokenType_PERCENT_EQ, /// %=

            TokenType_AND, /// and
            TokenType_OR, /// or
            TokenType_AND_BINARY, /// &
            TokenType_AND_BINARY_EQ, /// &=
            TokenType_OR_BINARY, /// |
            TokenType_OR_BINARY_EQ, /// |=
            TokenType_XOR, /// ^
            TokenType_XOR_EQ, /// ^=
            TokenType_TILDE, /// ~
            TokenType_TILDE_EQ, /// ~=

            TokenType_AT, /// @

            TokenType_COMMENT,
            TokenType_MULTILINE_COMMENT,

            TokenType_EOL_WIN, // \r\n
            TokenType_EOL_MAC, // \r
            TokenType_EOL_LIN, // \n
            TokenType_TAB,
            TokenType_SPACE,
        };

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

            std::string getType();

            std::string toString();
        };
    }

}

std::string e9lang::parser::Token::getType() {
    switch (type) {
        default:
            return "unknown";
        case TokenType_EOF:
            return "EOF";
    }
}

std::string e9lang::parser::Token::toString() {
    std::stringstream ss(getType());
    ss << "[" << begin.row << ',' << begin.col << "]: \"";
    if (type == TokenType_STRING)
        ss << '"';
    auto txt = text.size() > 40 ? (text.substr(0, 40) + "...") : text;
    for (auto &i : txt) {
        switch (i) {
            default:
                ss << i;
                break;
            case '\n':
                ss << "\\n";
                break;
            case '\r':
                ss << "\\r";
                break;
            case '\t':
                ss << "\\t";
                break;
            case '\b':
                ss << "\\b";
                break;
        }
    }
    ss << txt;
    if (type == TokenType_STRING)
        ss << '"';
    return ss.str();
}

#endif //E9LANG_PROJECT_TOKENS_HPP
