//
// Created by egor9814 on 13.03.18.
//

#ifndef E9LANG_PROJECT_LEXER_HPP
#define E9LANG_PROJECT_LEXER_HPP

#include "tokens.hpp"
#include <map>
#include <list>
#include <fstream>

namespace e9lang {

    namespace parser {

        struct LexerConfig {
            int tabWidth;
            bool skipComments;

            std::string operatorsChars;
            std::map<std::string, e9lang::parser::TokenType> operators;
            std::map<std::string, e9lang::parser::TokenType> keywords;

            LexerConfig(int tabWidth = 4, bool skipComments = true);
        };


        class SourceProvider {
        public:
            virtual bool open() = 0;

            virtual bool close() = 0;

            virtual std::string readAll() = 0;
        };

        class FileSourceProvider : public SourceProvider {
            std::string fileName;
            std::ifstream input;
        public:
            explicit FileSourceProvider(const std::string &fileName);

            bool open() override;

            bool close() override;

            std::string readAll() override;
        };


        class Lexer {
            LexerConfig &cfg;
            std::string source;
            unsigned long pos;
            unsigned long row, col;
            std::string buffer;
            std::list<e9lang::parser::Token *> tokens;
            std::list<e9lang::parser::Token *> errors;
            std::list<e9lang::parser::Token *> warnings;

            explicit Lexer(LexerConfig &config);

        public:
            Lexer(LexerConfig &config, const std::string &source);

            Lexer(LexerConfig &config, SourceProvider &sourceProvider);

            ~Lexer();

        private:
            struct State {
                unsigned long pos, row, col;
            };

            State save() const ;

            Token* addToken(TokenType type, State &begin, std::string error="");
            Token* addToken(TokenType type, std::string text, State &begin);

            void error(std::string text, State &begin);

            void warning(std::string text, State &begin);


            char peek(unsigned long relPos = 0);
            char next();


            void tokenizeOperator();

            void tokenizeNumber();

            void tokenizeWord();

            void tokenizeComment(bool multiline);

            void tokenizeString();

            void tokenizeChar();

        public:
            bool tokenize();

            std::list<e9lang::parser::Token *> getTokens() const;

            std::list<e9lang::parser::Token *> getErrors() const;

            std::list<e9lang::parser::Token *> getWarnings() const;
        };
    }

}

#endif //E9LANG_PROJECT_LEXER_HPP
