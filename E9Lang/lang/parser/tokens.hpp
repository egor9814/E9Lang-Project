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
        case TokenType_MINUS:
            return "Minus";
        case TokenType_MINUS_MINUS:
            return "MinusMinus";
        case TokenType_MINUS_EQ:
            return "MinusEq";
        case TokenType_STAR:
            return "Star";
        case TokenType_STAR_EQ:
            return "StarEq";
        case TokenType_STAR_STAR:
            return "StarStar";
        case TokenType_STAR_STAR_EQ:
            return "StrStarEq";
        case TokenType_SLASH:
            return "Slash";
        case TokenType_SLASH_EQ:
            return "SlashEq";
        case TokenType_PERCENT:
            return "Percent";
        case TokenType_PERCENT_EQ:
            return "PercentEq";
        case TokenType_AND:
            return "And";
        case TokenType_AND_BINARY:
            return "AndBin";
        case TokenType_AND_BINARY_EQ:
            return "AndBinEq";
        case TokenType_OR:
            return "Or";
        case TokenType_OR_BINARY:
            return "OrBin";
        case TokenType_OR_BINARY_EQ:
            return "OrBinEq";
        case TokenType_XOR:
            return "Xor";
        case TokenType_XOR_EQ:
            return "XorEq";
        case TokenType_TILDE:
            return "Tilde";
        case TokenType_TILDE_EQ:
            return "TildeEq";
        case TokenType_AT:
            return "At";
        case TokenType_COMMENT:
            return "Comment";
        case TokenType_MULTILINE_COMMENT:
            return "CommentM";
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
