//
// Created by egor9814 on 13.03.18.
//

#include "lexer.hpp"
#include "util.hpp"

e9lang::parser::LexerConfig::LexerConfig(int tabWidth, bool skipComments)
        : tabWidth(tabWidth), skipComments(skipComments),
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
    keywords["true"] = e9lang::parser::TokenType_TRUE;
    keywords["false"] = e9lang::parser::TokenType_FALSE;

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


e9lang::parser::FileSourceProvider::FileSourceProvider(const std::string &fileName)
        : fileName(fileName) {}

bool e9lang::parser::FileSourceProvider::open() {
    if (input.is_open())
        return false;
    input.open(fileName);
    return input.is_open();
}

bool e9lang::parser::FileSourceProvider::close() {
    if (!input.is_open())
        return false;
    input.close();
    return !input.is_open();
}

std::string e9lang::parser::FileSourceProvider::readAll() {
    std::stringstream ss;
    auto c = static_cast<char>(input.get());
    while (!input.eof()) {
        ss << c;
        c = static_cast<char>(input.get());
    }
    return ss.str();
}


e9lang::parser::Lexer::Lexer(e9lang::parser::LexerConfig &config)
        : cfg(config) {}

e9lang::parser::Lexer::Lexer(e9lang::parser::LexerConfig &config, const std::string &source)
        : cfg(config), source(source) {}

e9lang::parser::Lexer::Lexer(e9lang::parser::LexerConfig &config, e9lang::parser::SourceProvider &sourceProvider)
        : cfg(config), source(std::move(sourceProvider.readAll())) {}

e9lang::parser::Lexer::~Lexer() {
    e9lang::util::deallocate(tokens);
}


e9lang::parser::Lexer::State e9lang::parser::Lexer::save() const {
    return {pos, row, col - 1};
}

e9lang::parser::Token *e9lang::parser::Lexer::addToken(e9lang::parser::TokenType type,
                                                       e9lang::parser::Lexer::State &begin, std::string error) {
    auto t = new Token(type, {begin.pos, pos}, {begin.row, begin.col}, {row, col}, buffer, std::move(error));
    tokens.push_back(t);
    return t;
}

e9lang::parser::Token *e9lang::parser::Lexer::addToken(e9lang::parser::TokenType type,
                                                       std::string text, e9lang::parser::Lexer::State &begin) {
    auto t = new Token(type, {begin.pos, pos}, {begin.row, begin.col}, {row, col}, std::move(text));
    tokens.push_back(t);
    return t;
}

void e9lang::parser::Lexer::error(std::string text, e9lang::parser::Lexer::State &begin) {
    errors.push_back(addToken(TokenType_ERROR, begin, std::move(text)));
}

void e9lang::parser::Lexer::warning(std::string text, e9lang::parser::Lexer::State &begin) {
    warnings.push_back(addToken(TokenType_WARNING, begin, std::move(text)));
}


void e9lang::parser::Lexer::tokenizeOperator() {
    buffer.clear();
    auto state = save();
    char current = peek();
    if (current == '/') {
        if (peek(1) == '/') {
            tokenizeComment(false);
            return;
        } else if (peek(1) == '*') {
            tokenizeComment(true);
            return;
        }
    }
    while (true) {
        if (cfg.operators.count(buffer + current) == 0 && !buffer.empty()) {
            //pos--;
            addToken(cfg.operators[buffer], state);
            return;
        }
        buffer += current;
        current = next();
    }
}

void e9lang::parser::Lexer::tokenizeNumber() {
    buffer.clear();
    auto state = save();
    char current = peek();
    TokenType type = TokenType_INT;
    bool unsigned_ = false;
    bool typed = false;
    std::string error;
    while (true) {
        if (current == 'u' || current == 'U') {
            unsigned_ = true;
            current = next();
        } else if (current == 'c' || current == 'C') {
            if (typed && error.empty()) {
                error = "multitype not supported";
                current = next();
                continue;
            }
            type = TokenType_CHAR;
            typed = true;
            current = next();
        } else if (current == 's' || current == 'S') {
            if (typed && error.empty()) {
                error = "multitype not supported";
                current = next();
                continue;
            }
            type = TokenType_SHORT;
            typed = true;
            current = next();
        } else if (current == 'i' || current == 'I') {
            if (typed && error.empty()) {
                error = "multitype not supported";
                current = next();
                continue;
            }
            type = TokenType_INT;
            typed = true;
            current = next();
        } else if (current == 'l' || current == 'L') {
            if (typed && error.empty()) {
                error = "multitype not supported";
                current = next();
                continue;
            }
            type = TokenType_LONG;
            typed = true;
            current = next();
        } else if (current == 'f' || current == 'F') {
            if (typed && error.empty()) {
                error = "multitype not supported";
                current = next();
                continue;
            }
            type = TokenType_FLOAT;
            typed = true;
            if (unsigned_ && error.empty()) {
                error = "float cannot be is UNSIGNED";
            }
            current = next();
        } else if (current == 'd' || current == 'D') {
            if (typed && error.empty()) {
                error = "multitype not supported";
                current = next();
                continue;
            }
            type = TokenType_DOUBLE;
            typed = true;
            if (unsigned_ && error.empty()) {
                error = "double cannot be is UNSIGNED";
                current = next();
                continue;
            }
            current = next();
        } else if (current >= '0' && current <= '9') {
            buffer += current;
            current = next();
        } else if (current == '_') {
            current = next();
        } else if (current == '.'){
            if(buffer.find('.') != std::string::npos && error.empty()){
                error = "invalid double number";
            } else {
                buffer += current;
                type = TokenType_DOUBLE;
            }
            current = next();
        } else {
            break;
        }
    }
    if (error.empty()) {
        if (unsigned_) {
            switch (type) {
                case TokenType_CHAR:
                    type = TokenType_UCHAR;
                    break;
                case TokenType_SHORT:
                    type = TokenType_USHORT;
                    break;
                case TokenType_INT:
                    type = TokenType_UINT;
                    break;
                case TokenType_LONG:
                    type = TokenType_ULONG;
                    break;
            }
        }
        addToken(type, state);
    } else {
        this->error(std::move(error), state);
    }
}

inline bool isLetter(char &c) {
    return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

inline bool isIdentifierStart(char &c) {
    return c == '$' || c == '_' || isLetter(c);
}

inline bool isDigit(char &c) {
    return c >= '0' && c <= '9';
}

inline bool isIdentifierPart(char &c) {
    return isIdentifierStart(c) || isDigit(c);
}

void e9lang::parser::Lexer::tokenizeWord() {
    buffer.clear();
    auto state = save();
    buffer += peek();
    char current = next(); // skip first letter
    while (isIdentifierPart(current)) {
        buffer += current;
        current = next();
    }
    if (cfg.keywords.count(buffer) == 0) {
        addToken(TokenType_IDENTIFIER, state);
    } else {
        addToken(cfg.keywords[buffer], state);
    }
}

bool escapeOf(char &current, std::string &buffer, std::string &error, unsigned long &col) {
    switch (current) {
        default:
            error = "unknown escape symbol at " + std::to_string(col);
            break;
        case '0':
            buffer += '\0';
            break;
        case 'r':
            buffer += '\r';
            break;
        case 't':
            buffer += '\t';
            break;
        case 'a':
            buffer += '\a';
            break;
        case 'f':
            buffer += '\f';
            break;
        case 'v':
            buffer += '\v';
            break;
        case 'b':
            buffer += '\b';
            break;
        case 'n':
            buffer += '\n';
            break;
        case '"':
            buffer += '"';
            break;
        case '\'':
            buffer += '\'';
            break;
        case '\\':
            buffer += '\\';
            break;
    }
    return error.empty();
}

void e9lang::parser::Lexer::tokenizeString() {
    buffer.clear();
    auto state = save();
    std::string error;
    //buffer += peek();
    char current = next(); // skip open quote
    bool eof = false;
    while (true) {
        if (current == '"') {
            break;
        } else if (current == 0) {
            error = "unexpected end of file";
            eof = true;
            break;
        } else if (!error.empty()) {
            buffer += current;
            current = next();
        } else {
            if (current == '\\') {
                current = next();
                if (!escapeOf(current, buffer, error, col))
                    break;
            } else {
                buffer += current;
                current = next();
            }
        }
    }
    if (!eof) {
        //buffer += current;
        next(); // skip close quote
    }
    if (!error.empty()) {
        this->error(std::move(error), state);
    } else {
        addToken(TokenType_STRING, state);
    }
}

void e9lang::parser::Lexer::tokenizeChar() {
    buffer.clear();
    auto state = save();
    std::string error;
    //buffer += peek();
    char current = next(); // skip open quote
    bool eof = false;
    switch (current) {
        case 0:
            error = "unexpected end of file";
            eof = true;
            break;
        case '\\':
            current = next();
            escapeOf(current, buffer, error, col);
            break;
        default:
            buffer += current;
            break;
    }
    current = next();
    if (current != '\'') {
        error = "invalid close quote: ";
        error += current;
    }
    if (!eof) {
        //buffer += current;
        next(); // skip close quote
    }
    if (!error.empty()) {
        this->error(std::move(error), state);
    } else {
        addToken(TokenType_STRING, state);
    }
}

void e9lang::parser::Lexer::tokenizeComment(bool multiline) {
    buffer.clear();
    auto state = save();
    for (int i = 0; i < 2; i++) {
        buffer += peek();
        next();
    }

    std::string error;
    char current = peek();
    if (multiline) {
        //bool eof = false;
        while (current != '*' || peek(1) != '/') {
            if (current == 0) {
                error = "unexpected end of file";
                //eof = true;
                break;
            }
            buffer += current;
            current = next();
        }
    } else {
        while (current != 0 && row == state.row) {
            buffer += current;
            current = next();
        }
    }


    if (multiline)
        for (int i = 0; i < 2; i++) {
            buffer += peek();
            next();
        }

    if (error.empty()) {
        if (!cfg.skipComments)
            addToken(multiline ? TokenType_MULTILINE_COMMENT : TokenType_COMMENT, state);
    } else {
        this->error(error, state);
    }
}


char e9lang::parser::Lexer::peek(unsigned long relPos) {
    auto p = pos + relPos;
    if (p >= source.size()) {
        return 0;
    } else {
        return source[p];
    }
}

char e9lang::parser::Lexer::next() {
    pos++;
    char current = peek();
    if (current == '\r') {
        if (peek(1) == '\n') {
            pos++;
            current = peek();
        }
        col = 1;
        row++;
    } else if (current == '\n') {
        col = 1;
        row++;
    } else if (current == '\t') {
        col += cfg.tabWidth;
    } else {
        col++;
    }
    return current;
}


std::list<e9lang::parser::Token *> e9lang::parser::Lexer::getTokens() const {
    return tokens;
}

std::list<e9lang::parser::Token *> e9lang::parser::Lexer::getErrors() const {
    return errors;
}

std::list<e9lang::parser::Token *> e9lang::parser::Lexer::getWarnings() const {
    return warnings;
}


bool e9lang::parser::Lexer::tokenize() {
    e9lang::util::deallocate(tokens);
    errors.clear();
    warnings.clear();
    pos = static_cast<unsigned long>(-1);
    row = col = 1;

    next();
    while (true) {
        char current = peek();
        if (current == '\"')
            tokenizeString();
        else if (current == '\'')
            tokenizeChar();
        else if (isDigit(current))
            tokenizeNumber();
        else if (isIdentifierStart(current))
            tokenizeWord();
        else if (cfg.operatorsChars.find(current) != std::string::npos)
            tokenizeOperator();
        else if (current == 0)
            break;
        else
            next();
    }

    return errors.empty() && warnings.empty();
}











