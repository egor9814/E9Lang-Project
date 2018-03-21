//
// Created by egor9814 on 15.03.18.
//

#ifndef E9LANG_PROJECT_AST_HPP
#define E9LANG_PROJECT_AST_HPP

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
        public:
            virtual bool isAccessible() const { return false; }
        };

        class AccessibleExpression : public Expression {
        public:
            bool isAccessible() const override { return true; }
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

        class VarExpression : public AccessibleExpression {
            std::string &name;

        public:
            explicit VarExpression(Token *varName);

            std::string toString() override;

            void finalize() override;
        };

        class AssignExpression : public Expression {
            AccessibleExpression *target;
            Expression *value;
            Token *operation;

        public:
            AssignExpression(AccessibleExpression *target, Expression *value, Token *operation);

            std::string toString() override;

            void finalize() override;
        };

        class FunctionCallExpression : public Expression {
            Expression *name;
            std::list<Expression *> args;

        public:
            explicit FunctionCallExpression(Expression *name);

            void addArgument(Expression *arg);

            std::string toString() override;

            void finalize() override;
        };

        class TernaryExpression : public Expression {
            Expression *condition, *trueExpression, *falseExpression;
        public:
            TernaryExpression(Expression *condition, Expression *trueExpression, Expression *falseExpression);

            std::string toString() override;

            void finalize() override;
        };

        class ElvisExpression : public Expression {
            Expression *condition, *value;
        public:
            ElvisExpression(Expression *condition, Expression *value);

            std::string toString() override;

            void finalize() override;
        };

        class ContainerAccessExpression : public AccessibleExpression {
            std::list<Expression *> indexes;
        public:
            explicit ContainerAccessExpression(std::list<Expression *> &indexes);

            ContainerAccessExpression(Expression *target, std::list<Expression *> &indexes);

            explicit ContainerAccessExpression(Expression *target);

            std::string toString() override;

            void finalize() override;
        };


        class Argument : public Node {
            std::string name;
            bool var_arg;
        public:
            Argument(Token *name, bool var_arg);

            std::string &getName();

            bool isVarArg() const;

            std::string toString() override;

            void finalize() override;
        };

        class Arguments : public Node {
            std::list<Argument *> args{};
        public:
            Arguments() = default;

            void add(Argument *arg);

            bool hasVarArgs();

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
            std::list<Token *> names;
            std::list<Expression *> values;

        public:
            VarStatement(std::list<Token *> &names, std::list<Expression *> &values);

            std::string toString() override;

            void finalize() override;
        };

        class ConstStatement : public Statement {
            std::list<Token *> names;
            std::list<Expression *> values;

        public:
            ConstStatement(std::list<Token *> &names, std::list<Expression *> &values);

            std::string toString() override;

            void finalize() override;
        };

        class PrintStatement : public Statement {
            Expression *message;

        public:
            explicit PrintStatement(Expression *message);

            std::string toString() override;

            void finalize() override;
        };

        class FunStatement : public Statement {
            Token *name;
            Arguments *args;
            Statement *body;

        public:
            FunStatement(Token *functionName, Arguments *args, Statement *body);

            std::string toString() override;

            void finalize() override;
        };

        class BlockStatement : public Statement {
            std::list<e9lang::ast::Statement *> statements{};

        public:
            BlockStatement() = default;

            void add(Statement *);

            std::string toString() override;

            void finalize() override;
        };

        class ReturnStatement : public Statement {
            Expression *value;
        public:
            explicit ReturnStatement(Expression *value);

            std::string toString() override;

            void finalize() override;
        };

        class LoopControlStatement : public Statement {
            Token *type;

        public:
            explicit LoopControlStatement(Token *type);

            std::string toString() override;

            void finalize() override;
        };

        class WhileLoop : public Statement {
            bool postCondition;
            Expression *condition;
            Statement *body;

        public:
            WhileLoop(bool postCondition, Expression *condition, Statement *body);

            std::string toString() override;

            void finalize() override;
        };

        class ForLoop : public Statement {
            Statement *init;
            Expression *condition;
            Statement *update;
            Statement *body;

        public:
            ForLoop(Statement *init, Expression *condition, Statement *update, Statement *body);

            std::string toString() override;

            void finalize() override;
        };

        class EmptyStatement : public Statement {
        public:
            EmptyStatement();

            std::string toString() override;

            void finalize() override;
        };

        class ExpressionStatement : public Statement {
            Expression *expression;
        public:
            explicit ExpressionStatement(Expression *expression);

            std::string toString() override;

            void finalize() override;
        };
    }

}

#endif //PROJECT_AST_HPP
