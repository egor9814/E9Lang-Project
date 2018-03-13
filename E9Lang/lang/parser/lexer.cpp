//
// Created by egor9814 on 13.03.18.
//

#include "lexer.hpp"
#include "util.hpp"

e8lang::parser::LexerConfig::LexerConfig(int tabWidth, bool skipWS, bool skipEOL, bool skipComments)
        : tabWidth(tabWidth), skipWS(skipWS), skipEOL(skipEOL), skipComments(skipComments),
          operatorsChars("[]{}(),.;:<>?!~@%^&*/-+=|") {
    operators["."] = e9lang::parser::TokenType_DOT;
    operators[","] = e9lang::parser::TokenType_COMMA;
    operators[":"] = e9lang::parser::TokenType_COLON;
    operators[";"] = e9lang::parser::TokenType_SEMICOLON;
    operators["::"] = e9lang::parser::TokenType_COLON_COLON;
    operators["("] = e9lang::parser::TokenType_LPAREN;
    operators[")"] = e9lang::parser::TokenType_RPAREN;
    operators["["] = e9lang::parser::TokenType_LBRACKET;
    operators["]"] = e9lang::parser::TokenType_RBRACKET;
    operators["{"] = e9lang::parser::TokenType_LBRACE;
    operators["}"] = e9lang::parser::TokenType_RBRACE;
    operators["="] = e9lang::parser::TokenType_EQ;
    operators["=="] = e9lang::parser::TokenType_EQ_EQ;
    operators["!"] = e9lang::parser::TokenType_EXCL;
    operators["!="] = e9lang::parser::TokenType_EXCL_EQ;
    operators["?"] = e9lang::parser::TokenType_HOOK;
    operators["?="] = e9lang::parser::TokenType_HOOK_EQ;
    operators["<"] = e9lang::parser::TokenType_LT;
    operators["<="] = e9lang::parser::TokenType_LT_EQ;
    operators[">"] = e9lang::parser::TokenType_GT;
    operators[">="] = e9lang::parser::TokenType_GT_EQ;
    operators["<<"] = e9lang::parser::TokenType_SHIFT_LEFT;
    operators["<<<"] = e9lang::parser::TokenType_U_SHIFT_LEFT;
    operators["<<="] = e9lang::parser::TokenType_SHIFT_LEFT_EQ;
    operators["<<<="] = e9lang::parser::TokenType_U_SHIFT_LEFT_EQ;
    operators[">>"] = e9lang::parser::TokenType_SHIFT_RIGHT;
    operators[">>>"] = e9lang::parser::TokenType_U_SHIFT_RIGHT;
    operators[">>="] = e9lang::parser::TokenType_SHIFT_RIGHT_EQ;
    operators[">>>="] = e9lang::parser::TokenType_U_SHIFT_RIGHT_EQ;
    operators["+"] = e9lang::parser::TokenType_PLUS;
    operators["++"] = e9lang::parser::TokenType_PLUS_PLUS;
    operators["+="] = e9lang::parser::TokenType_PLUS_EQ;
    operators["-"] = e9lang::parser::TokenType_MINUS;
    operators["--"] = e9lang::parser::TokenType_MINUS_MINUS;
    operators["-="] = e9lang::parser::TokenType_MINUS_EQ;
    operators["*"] = e9lang::parser::TokenType_STAR;
    operators["**"] = e9lang::parser::TokenType_STAR_STAR;
    operators["*="] = e9lang::parser::TokenType_STAR_EQ;
    operators["**="] = e9lang::parser::TokenType_STAR_STAR_EQ;
    operators["/"] = e9lang::parser::TokenType_SLASH;
    operators["/="] = e9lang::parser::TokenType_SLASH_EQ;
    operators["%"] = e9lang::parser::TokenType_PERCENT;
    operators["%="] = e9lang::parser::TokenType_PERCENT_EQ;
    keywords["and"] = e9lang::parser::TokenType_AND;
    operators["&"] = e9lang::parser::TokenType_AND_BINARY;
    operators["&="] = e9lang::parser::TokenType_AND_BINARY_EQ;
    keywords["or"] = e9lang::parser::TokenType_OR;
    operators["|"] = e9lang::parser::TokenType_OR_BINARY;
    operators["|="] = e9lang::parser::TokenType_OR_BINARY_EQ;
    operators["^"] = e9lang::parser::TokenType_XOR;
    operators["^="] = e9lang::parser::TokenType_XOR_EQ;
    operators["~"] = e9lang::parser::TokenType_TILDE;
    operators["~="] = e9lang::parser::TokenType_TILDE_EQ;
    operators["@"] = e9lang::parser::TokenType_AT;

    keywords["null"] = e9lang::parser::TokenType_NULL;
    keywords["if"] = e9lang::parser::TokenType_IF;
    keywords["else"] = e9lang::parser::TokenType_ELSE;
    keywords["for"] = e9lang::parser::TokenType_FOR;
    keywords["while"] = e9lang::parser::TokenType_WHILE;
    keywords["do"] = e9lang::parser::TokenType_DO;
    keywords["continue"] = e9lang::parser::TokenType_CONTINUE;
    keywords["break"] = e9lang::parser::TokenType_BREAK;
    keywords["return"] = e9lang::parser::TokenType_RETURN;
    keywords["fun"] = e9lang::parser::TokenType_FUN;
    keywords["var"] = e9lang::parser::TokenType_VAR;
    keywords["const"] = e9lang::parser::TokenType_CONST;
    keywords["print"] = e9lang::parser::TokenType_PRINT;
}


e8lang::parser::FileSourceProvider::FileSourceProvider(const std::string &fileName) : fileName(fileName) {}

bool e8lang::parser::FileSourceProvider::open() {
    if (input.is_open())
        return false;
    input.open(fileName);
    return input.is_open();
}

bool e8lang::parser::FileSourceProvider::close() {
    if (!input.is_open())
        return false;
    input.close();
    return !input.is_open();
}

std::string e8lang::parser::FileSourceProvider::readAll() {
    std::stringstream ss;
    auto c = static_cast<char>(input.get());
    while (!input.eof()) {
        ss << c;
        c = static_cast<char>(input.get());
    }
    return ss.str();
}


e8lang::parser::Lexer::Lexer(e8lang::parser::LexerConfig &config)
        : cfg(config) {}

e8lang::parser::Lexer::Lexer(e8lang::parser::LexerConfig &config, const std::string &source)
        : cfg(config), source(source) {}

e8lang::parser::Lexer::Lexer(e8lang::parser::LexerConfig &config, e8lang::parser::SourceProvider &sourceProvider)
        : cfg(config), source(std::move(sourceProvider.readAll())) {}

e8lang::parser::Lexer::~Lexer() {
    e9lang::util::deallocate(tokens);
}
