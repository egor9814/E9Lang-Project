//
// Created by egor9814 on 13.03.18.
//

#ifndef E9LANG_PROJECT_TOKENS_HPP
#define E9LANG_PROJECT_TOKENS_HPP

#include <string>
#include <sstream>
#include <map>
#include "enum.hpp"

namespace e9lang {

    namespace parser {

        enum TokenType {

            TokenType_EOF = 0, /// EOF

            TokenType_ERROR, /// for lexing errors
            TokenType_WARNING, /// for lexing warnings

            TokenType_NULL, /// null reference
            TokenType_CHAR, TokenType_UCHAR, /// signed/unsigned char types
            TokenType_SHORT, TokenType_USHORT, /// signed/unsigned short types
            TokenType_INT, TokenType_UINT, /// signed/unsigned int types
            TokenType_LONG, TokenType_ULONG, /// signed/unsigned long types
            TokenType_FLOAT, TokenType_DOUBLE, /// float-point number types
            TokenType_TRUE, TokenType_FALSE, /// bool type
            TokenType_STRING, /// string type
            //TokenType_ARRAY, TokenType_MAP,
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
            std::string errorText;

            Token(TokenType type, Range range, Location begin, Location end,
                  const std::string &text, const std::string &errorText = "");

            std::string getType();

            std::string toString();
        };
    }

}

e9lang::parser::Token::Token(e9lang::parser::TokenType type, e9lang::parser::Range range,
                             e9lang::parser::Location begin, e9lang::parser::Location end, const std::string &text,
                             const std::string &errorText)
        : type(type), range(range), begin(begin), end(end),
          text(std::move(text)), errorText(std::move(errorText)) {}

std::string e9lang::parser::Token::getType() {
    switch (type) {
        default:
            return "unknown";
        case TokenType_EOF:
            return "EOF";
        case TokenType_ERROR:
            return "Error";
        case TokenType_WARNING:
            return "Warning";
        case TokenType_NULL:
            return "Null";
        case TokenType_CHAR:
            return "Char";
        case TokenType_UCHAR:
            return "UChar";
        case TokenType_SHORT:
            return "Short";
        case TokenType_USHORT:
            return "UShort";
        case TokenType_INT:
            return "Int";
        case TokenType_UINT:
            return "UInt";
        case TokenType_LONG:
            return "Long";
        case TokenType_ULONG:
            return "ULong";
        case TokenType_FLOAT:
            return "Float";
        case TokenType_DOUBLE:
            return "Double";
        case TokenType_TRUE:
            return "True";
        case TokenType_FALSE:
            return "False";
        case TokenType_STRING:
            return "String";
        case TokenType_IDENTIFIER:
            return "Identifier";
        case TokenType_IF:
            return "If";
        case TokenType_ELSE:
            return "Else";
        case TokenType_FOR:
            return "For";
        case TokenType_DO:
            return "Do";
        case TokenType_WHILE:
            return "While";
        case TokenType_CONTINUE:
            return "Continue";
        case TokenType_BREAK:
            return "Break";
        case TokenType_RETURN:
            return "Return";
        case TokenType_FUN:
            return "Fun";
        case TokenType_VAR:
            return "Var";
        case TokenType_CONST:
            return "Const";
        case TokenType_PRINT:
            return "Print";
        case TokenType_DOT:
            return "Dot";
        case TokenType_COMMA:
            return "Comma";
        case TokenType_COLON:
            return "Colon";
        case TokenType_COLON_COLON:
            return "ColonColon";
        case TokenType_SEMICOLON:
            return "Semicolon";
        case TokenType_LPAREN:
            return "LParen";
        case TokenType_RPAREN:
            return "RParen";
        case TokenType_LBRACKET:
            return "LBracket";
        case TokenType_RBRACKET:
            return "RBracket";
        case TokenType_LBRACE:
            return "LBrace";
        case TokenType_RBRACE:
            return "RBrace";
        case TokenType_EQ:
            return "Eq";
        case TokenType_EQ_EQ:
            return "EqEq";
        case TokenType_EXCL:
            return "Excl";
        case TokenType_EXCL_EQ:
            return "ExclEq";
        case TokenType_HOOK:
            return "Hook";
        case TokenType_HOOK_EQ:
            return "HookEq";
        case TokenType_LT:
            return "LT";
        case TokenType_LT_EQ:
            return "LTEq";
        case TokenType_GT:
            return "GT";
        case TokenType_GT_EQ:
            return "GTEq";
        case TokenType_SHIFT_LEFT:
            return "ShiftLeft";
        case TokenType_SHIFT_LEFT_EQ:
            return "ShiftLeftEq";
        case TokenType_U_SHIFT_LEFT:
            return "UShiftLeft";
        case TokenType_U_SHIFT_LEFT_EQ:
            return "UShiftLeftEq";
        case TokenType_SHIFT_RIGHT:
            return "ShiftRight";
        case TokenType_SHIFT_RIGHT_EQ:
            return "ShiftRightEq";
        case TokenType_U_SHIFT_RIGHT:
            return "UShiftRight";
        case TokenType_U_SHIFT_RIGHT_EQ:
            return "UShiftRightEq";
        case TokenType_PLUS:
            return "Plus";
        case TokenType_PLUS_PLUS:
            return "PlusPlus";
        case TokenType_PLUS_EQ:
            return "PlusEq";
    }
}

std::string e9lang::parser::Token::toString() {
    std::stringstream ss;
    ss << getType();
    ss << "[" << begin.row << ',' << begin.col << "]: ";
    if(type == TokenType_ERROR || type == TokenType_WARNING)
        ss << errorText << " -> ";
    ss << '"';
    if (type == TokenType_STRING)
        ss << '"';
    auto txt = text.size() > 40 ? (text.substr(0, 40) + "...") : text;
    for (auto &i : txt) {
        switch (i) {
            default:
                ss << i;
                break;
            case '\0':
                ss << "\\0";
                break;
            case '\r':
                ss << "\\r";
                break;
            case '\t':
                ss << "\\t";
                break;
            case '\a':
                ss << "\\a";
                break;
            case '\f':
                ss << "\\f";
                break;
            case '\v':
                ss << "\\v";
                break;
            case '\b':
                ss << "\\b";
                break;
            case '\n':
                ss << "\\n";
                break;
            case '\"':
                ss << "\\\"";
                break;
            case '\'':
                ss << "\\'";
                break;
            case '\\':
                ss << "\\\\";
                break;
        }
    }
    if (type == TokenType_STRING)
        ss << '"';
    ss << '"';
    return ss.str();
}

#endif //E9LANG_PROJECT_TOKENS_HPP
