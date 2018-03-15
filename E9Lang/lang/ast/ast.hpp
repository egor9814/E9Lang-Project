//
// Created by egor9814 on 15.03.18.
//

#ifndef E9LANG_PROJECT_AST_HPP
#define E9LANG_PROJECT_AST_HPP

#include <string>
#include <list>
#include "../parser/tokens.hpp"

namespace e9lang {

    namespace ast {

        using namespace e9lang::parser;

        class Node {
        public:
            virtual std::string toString() = 0;

            virtual void finalize() = 0;
        };


        class Expression : public Node {
        };

        class UnaryExpression : public Expression {
            Expression *target;
            Token *operation;

        public:
            UnaryExpression(Expression *target, Token *operation);

            std::string toString() override;

            void finalize() override;
        };

        class BinaryExpression : public Expression {
            Expression *left, *right;
            Token *operation;

        public:
            BinaryExpression(Expression *left, Expression *right, Token *operation);

            std::string toString() override;

            void finalize() override;
        };

        class ValueExpression : public Expression {
            Token *value;

        public:
            explicit ValueExpression(Token *value);

            std::string toString() override;

            void finalize() override;
        };

        class VarExpression : public Expression {
            std::string &name;

        public:
            explicit VarExpression(Token *varName);

            std::string toString() override;

            void finalize() override;
        };


        class Statement : public Node {
        };

        class IfStatement : public Statement {
            Expression *condition;
            Statement *thenStatement, *elseStatement;

        public:
            IfStatement(Expression *condition, Statement *thenStatement, Statement *elseStatement);

            std::string toString() override;

            void finalize() override;
        };

        class VarStatement : public Statement {
            std::list<Token *> names, values;

        public:
            VarStatement(std::list<Token *> &names, std::list<Token *> &values);

            std::string toString() override;

            void finalize() override;
        };

        class ConstStatement : public Statement {
            std::list<Token *> names, values;

        public:
            ConstStatement(std::list<Token *> &names, std::list<Token *> &values);

            std::string toString() override;

            void finalize() override;
        };

        class PrintStatement : public Statement {
            Expression* message;

        public:
            explicit PrintStatement(Expression* message);

            std::string toString() override;

            void finalize() override;
        };

        class FunStatement : public Statement {
            Token* name;
            std::list<Token*> args;
            Statement* body;

        public:
            FunStatement(Token* functionName, std::list<Token*>& args, Statement* body);

            std::string toString() override;

            void finalize() override;
        };

        class BlockStatement : public Statement {
            std::list<e9lang::ast::Statement*> statements;

        public:
            BlockStatement() = default;

            void add(Statement*);

            std::string toString() override;
        };
    }

}

#endif //PROJECT_AST_HPP
