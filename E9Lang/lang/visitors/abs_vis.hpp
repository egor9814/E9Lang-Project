//
// Created by egor9814 on 27.03.18.
//

#ifndef E9LANG_PROJECT_ABS_VIS_HPP
#define E9LANG_PROJECT_ABS_VIS_HPP

#include <e9lang/ast.hpp>

class AbstractVisitor : public e9lang::ast::Visitor {
public:
    void visit(e9lang::ast::Node *node) override;

    void visit(e9lang::ast::Unit *unit) override;

    void visit(e9lang::ast::UnaryExpression *e) override;

    void visit(e9lang::ast::BinaryExpression *e) override;

    void visit(e9lang::ast::ValueExpression *e) override;

    void visit(e9lang::ast::VarExpression *e) override;

    void visit(e9lang::ast::AssignExpression *e) override;

    void visit(e9lang::ast::FunctionCallExpression *e) override;

    void visit(e9lang::ast::TernaryExpression *e) override;

    void visit(e9lang::ast::ElvisExpression *e) override;

    void visit(e9lang::ast::ContainerAccessExpression *e) override;

    void visit(e9lang::ast::ArrayExpression *e) override;

    void visit(e9lang::ast::MapExpression *e) override;

    void visit(e9lang::ast::Argument *a) override;

    void visit(e9lang::ast::Arguments *a) override;

    void visit(e9lang::ast::IfStatement *s) override;

    void visit(e9lang::ast::VarStatement *s) override;

    void visit(e9lang::ast::ConstStatement *s) override;

    void visit(e9lang::ast::PrintStatement *s) override;

    void visit(e9lang::ast::FunStatement *s) override;

    void visit(e9lang::ast::BlockStatement *s) override;

    void visit(e9lang::ast::ReturnStatement *s) override;

    void visit(e9lang::ast::LoopControlStatement *s) override;

    void visit(e9lang::ast::WhileLoop *s) override;

    void visit(e9lang::ast::ForLoop *s) override;

    void visit(e9lang::ast::EmptyStatement *s) override;

    void visit(e9lang::ast::ExpressionStatement *s) override;
};

#endif //E9LANG_PROJECT_ABS_VIS_HPP
