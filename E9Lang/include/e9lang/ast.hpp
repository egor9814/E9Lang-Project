//
// Created by egor9814 on 15.03.18.
//

#ifndef E9LANG_PROJECT_AST_HPP
#define E9LANG_PROJECT_AST_HPP

#include <list>
#include <vector>
#include "tokens.hpp"

namespace e9lang {

    namespace ast {

        using namespace e9lang::parser;

        class Visitor;

        class Node {//}; : public Visitable {
        public:
            virtual std::string toString() = 0;

            virtual void finalize() = 0;

            virtual void accept(Visitor *visitor) = 0;
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
        public:
            Expression *target;
            Token *operation;

            UnaryExpression(Expression *target, Token *operation);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class BinaryExpression : public Expression {
        public:
            Expression *left, *right;
            Token *operation;

            BinaryExpression(Expression *left, Expression *right, Token *operation);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class ValueExpression : public Expression {
        public:
            std::string value;
            TokenType type;

            explicit ValueExpression(Token *value);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class VarExpression : public AccessibleExpression {
        public:
            std::string name;

            explicit VarExpression(Token *varName);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class AssignExpression : public Expression {
        public:
            AccessibleExpression *target;
            Expression *value;
            Token *operation;

            AssignExpression(AccessibleExpression *target, Expression *value, Token *operation);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class FunctionCallExpression : public Expression {
        public:
            Expression *name;
            std::vector<Expression *> args;

            explicit FunctionCallExpression(Expression *name);

            void addArgument(Expression *arg);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class TernaryExpression : public Expression {
        public:
            Expression *condition, *trueExpression, *falseExpression;
            TernaryExpression(Expression *condition, Expression *trueExpression, Expression *falseExpression);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class ElvisExpression : public Expression {
        public:
            Expression *condition, *value;
            ElvisExpression(Expression *condition, Expression *value);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class ContainerAccessExpression : public AccessibleExpression {
        public:
            std::vector<Expression *> indexes;
            explicit ContainerAccessExpression(std::vector<Expression *> &indexes);

            ContainerAccessExpression(Expression *target, std::vector<Expression *> &indexes);

            explicit ContainerAccessExpression(Expression *target);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class ArrayExpression : public Expression {
        public:
            std::list<Expression *> values;
            explicit ArrayExpression(std::list<Expression *> &values);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class MapExpression : public Expression {
        public:
            std::list<Expression *> keys, values;
            MapExpression(std::list<Expression *> &keys, std::list<Expression *> &values);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };


        class Argument : public Node {
        public:
            std::string name;
            bool var_arg;
            Argument(Token *name, bool var_arg);

            std::string &getName();

            bool isVarArg() const;

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class Arguments : public Node {
        public:
            std::list<Argument *> args{};
            Arguments() = default;

            void add(Argument *arg);

            bool hasVarArgs();

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };


        class Statement : public Node {
        };

        class IfStatement : public Statement {
        public:
            Expression *condition;
            Statement *thenStatement, *elseStatement;

            IfStatement(Expression *condition, Statement *thenStatement, Statement *elseStatement);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class VarStatement : public Statement {
        public:
            std::list<Token *> names;
            std::list<Expression *> values;

            VarStatement(std::list<Token *> &names, std::list<Expression *> &values);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class ConstStatement : public Statement {
        public:
            std::list<Token *> names;
            std::list<Expression *> values;

            ConstStatement(std::list<Token *> &names, std::list<Expression *> &values);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class PrintStatement : public Statement {
        public:
            Expression *message;

            explicit PrintStatement(Expression *message);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class FunStatement : public Statement, public Expression {
        public:
            Token *name;
            Arguments *args;
            Statement *body;

            FunStatement(Token *functionName, Arguments *args, Statement *body);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class BlockStatement : public Statement {
        public:
            std::list<e9lang::ast::Statement *> statements{};

            BlockStatement() = default;

            void add(Statement *);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class ReturnStatement : public Statement {
        public:
            Expression *value;
            explicit ReturnStatement(Expression *value);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class LoopControlStatement : public Statement {
        public:
            Token *type;

            explicit LoopControlStatement(Token *type);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class WhileLoop : public Statement {
        public:
            bool postCondition;
            Expression *condition;
            Statement *body;

            WhileLoop(bool postCondition, Expression *condition, Statement *body);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class ForLoop : public Statement {
        public:
            Statement *init;
            Expression *condition;
            Statement *update;
            Statement *body;

            ForLoop(Statement *init, Expression *condition, Statement *update, Statement *body);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class EmptyStatement : public Statement {
        public:
            EmptyStatement();

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };

        class ExpressionStatement : public Statement {
        public:
            Expression *expression;
            explicit ExpressionStatement(Expression *expression);

            std::string toString() override;

            void finalize() override;

            void accept(Visitor *visitor) override;
        };


        class Unit : public ast::Node {
        public:
            std::list<e9lang::ast::FunStatement *> functions;

            explicit Unit(std::list<e9lang::ast::FunStatement *> &functions);

            std::string toString() override;

            void finalize() override;

            void accept(ast::Visitor *visitor) override;
        };



        class Visitor {
        public:
            virtual void visit(Node* node) = 0;

            virtual void visit(Unit* unit) = 0;

            virtual void visit(UnaryExpression* e) = 0;
            virtual void visit(BinaryExpression* e) = 0;
            virtual void visit(ValueExpression* e) = 0;
            virtual void visit(VarExpression* e) = 0;
            virtual void visit(AssignExpression* e) = 0;
            virtual void visit(FunctionCallExpression* e) = 0;
            virtual void visit(TernaryExpression* e) = 0;
            virtual void visit(ElvisExpression* e) = 0;
            virtual void visit(ContainerAccessExpression* e) = 0;
            virtual void visit(ArrayExpression* e) = 0;
            virtual void visit(MapExpression* e) = 0;

            virtual void visit(Argument* a) = 0;
            virtual void visit(Arguments* a) = 0;

            virtual void visit(IfStatement* s) = 0;
            virtual void visit(VarStatement* s) = 0;
            virtual void visit(ConstStatement* s) = 0;
            virtual void visit(PrintStatement* s) = 0;
            virtual void visit(FunStatement* s) = 0;
            virtual void visit(BlockStatement* s) = 0;
            virtual void visit(ReturnStatement* s) = 0;
            virtual void visit(LoopControlStatement* s) = 0;
            virtual void visit(WhileLoop* s) = 0;
            virtual void visit(ForLoop* s) = 0;
            virtual void visit(EmptyStatement* s) = 0;
            virtual void visit(ExpressionStatement* s) = 0;
        };
/*        template <> struct Model<UnaryExpression> {
            static void visit(UnaryExpression* visitable, Visitor* visitor){
                visitable->target->accept(visitor);
            }
        };

#define CREATE_AST_MODEL(NodeType, Name, Unit)\
        template <> struct Model<NodeType> {\
            static void visit(NodeType* Name, Visitor* v) Unit\
        };

        CREATE_AST_MODEL(BinaryExpression, e, {
            e->left->accept(v);
            e->right->accept(v);
        })
        CREATE_AST_MODEL(ValueExpression, e, {})
        CREATE_AST_MODEL(VarExpression, e, {})
        CREATE_AST_MODEL(AssignExpression, e, {
            e->target->accept(v);
            e->value->accept(v);
        })
        CREATE_AST_MODEL(FunctionCallExpression, e, {
            e->name->accept(v);
            for(auto &i : e->args){
                i->accept(v);
            }
        })
        CREATE_AST_MODEL(TernaryExpression, e, {
            e->condition->accept(v);
            e->trueExpression->accept(v);
            e->falseExpression->accept(v);
        })
        CREATE_AST_MODEL(ElvisExpression, e, {
            e->condition->accept(v);
            e->value->accept(v);
        })
        CREATE_AST_MODEL(ContainerAccessExpression, e, {
            for(auto &i : e->indexes){
                i->accept(v);
            }
        })
        CREATE_AST_MODEL(ArrayExpression, e, {
            for(auto &i : e->values){
                i->accept(v);
            }
        })
        CREATE_AST_MODEL(MapExpression, e, {
            auto value = e->values.begin();
            auto end = e->keys.end();
            for(auto key = e->keys.begin(); key != end; key++, value++){
                (*key)->accept(v);
                (*value)->accept(v);
            }
        })

        CREATE_AST_MODEL(Argument, a, {})
        CREATE_AST_MODEL(Arguments, a, {
            for(auto &i : a->args){
                i->accept(v);
            }
        })

        CREATE_AST_MODEL(IfStatement, s, {
            s->condition->accept(v);
            s->thenStatement->accept(v);
            if(s->elseStatement)
                s->elseStatement->accept(v);
        })
        CREATE_AST_MODEL(VarStatement, s, {
            for(auto &i : s->values){
                i->accept(v);
            }
        })
        CREATE_AST_MODEL(ConstStatement, s, {
            for(auto &i : s->values){
                s->accept(v);
            }
        })
        CREATE_AST_MODEL(PrintStatement, s, {
            s->message->accept(v);
        })
        CREATE_AST_MODEL(FunStatement, s, {
            s->args->accept(v);
            s->body->accept(v);
        })
        CREATE_AST_MODEL(BlockStatement, s, {
            for(auto &i : s->statements){
                i->accept(v);
            }
        })
        CREATE_AST_MODEL(ReturnStatement, s, {
            s->value->accept(v);
        })
        CREATE_AST_MODEL(LoopControlStatement, s, {})
        CREATE_AST_MODEL(WhileLoop, l, {
            l->condition->accept(v);
            l->body->accept(v);
        })
        CREATE_AST_MODEL(ForLoop, l, {
            if(l->init)
                l->init->accept(v);
            if(l->condition)
                l->condition->accept(v);
            if(l->update)
                l->update->accept(v);
            l->body->accept(v);
        })
        CREATE_AST_MODEL(EmptyStatement, s, {})
        CREATE_AST_MODEL(ExpressionStatement, s, {
            s->expression->accept(v);
        })

#undef CREATE_AST_MODEL*/
    }
}

#endif //PROJECT_AST_HPP
