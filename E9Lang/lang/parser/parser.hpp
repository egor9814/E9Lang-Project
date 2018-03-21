//
// Created by egor9814 on 15.03.18.
//

#ifndef E9LANG_PROJECT_PARSER_HPP
#define E9LANG_PROJECT_PARSER_HPP

#include <set>
#include <vector>
#include "../ast/ast.hpp"

namespace e9lang {

    namespace parser {

        class Parser;

        class Unit : public ast::Node {
            friend class Parser;

            std::list<e9lang::ast::FunStatement *> functions;

            explicit Unit(std::list<e9lang::ast::FunStatement *> &functions);

        public:
            std::string toString() override;

            void finalize() override;
        };


        class Parser {
        public:
            typedef std::vector<e9lang::parser::Token *> TokenList;
        private:
            e9lang::parser::Token *eofToken;
            TokenList tokens;
            unsigned long pos;

            std::set<e9lang::parser::TokenType> assignOperators;

            Unit *parsedUnit = nullptr;
            TokenList errors;

            void error(std::string text, Token *begin, Token *end);

        public:
            explicit Parser(std::list<e9lang::parser::Token *> &tokens);

            ~Parser();

        private:
            Token *get(unsigned long relPos = 0);

            Token *require(TokenType type);

            bool match(TokenType type);

            bool lookMatch(unsigned long relPos, TokenType type);


            ast::Expression *expression();


            ast::Expression *value();

            std::list<ast::Expression *> qualifier();

            ast::Expression *qualifiedName();

            ast::Expression *variable();

            ast::Expression *primary();

            ast::Expression *unary();

            ast::Expression *multiplicative();

            ast::Expression *additive();

            ast::Expression *shift();

            ast::Expression *conditional();

            ast::Expression *equality();

            ast::Expression *bitwiseAnd();

            ast::Expression *bitwiseXor();

            ast::Expression *bitwiseOr();

            ast::Expression *logicalAnd();

            ast::Expression *logicalOr();

            ast::Expression *ternary();

            ast::Expression *assignmentStrict();

            ast::Expression *assignment();


            ast::FunctionCallExpression *functionCall(ast::Expression *name);

            ast::Expression *functionCalls(ast::Expression *name);

            ast::Arguments *arguments();

            ast::FunStatement *functionDefine();


            ast::Statement *statement();

            ast::Statement *statementBlock();

            ast::Statement *statementOrBlock();

            ast::IfStatement *ifStatement();

            void varOrConstDeclaration(std::list<Token *> &names, std::list<ast::Expression *> &values);

            ast::VarStatement *varDeclaration();

            ast::ConstStatement *constDeclaration();

            ast::PrintStatement *print();

            ast::ReturnStatement *returnStatement();

            ast::ForLoop *forLoop();

            ast::WhileLoop *whileLoop();

            ast::WhileLoop *doWhileLoop();


        public:
            Unit *getUnit() const ;

            bool parse();

            TokenList getErrors() const ;
        };
    }

}

#endif //E9LANG_PROJECT_PARSER_HPP
