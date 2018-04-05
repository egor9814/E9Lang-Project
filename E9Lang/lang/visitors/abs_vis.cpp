//
// Created by egor9814 on 27.03.18.
//

#include "abs_vis.hpp"

void AbstractVisitor::visit(e9lang::ast::Node *node) {
    throw "cannot visit abstract node";
}

void AbstractVisitor::visit(e9lang::ast::UnaryExpression *e) {
    e->target->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::BinaryExpression *e) {
    e->left->accept(this);
    e->right->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::ValueExpression *e) {
}

void AbstractVisitor::visit(e9lang::ast::VarExpression *e) {
}

void AbstractVisitor::visit(e9lang::ast::AssignExpression *e) {
    e->target->accept(this);
    e->value->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::FunctionCallExpression *e) {
    e->name->accept(this);
    for(auto &i : e->args){
        i->accept(this);
    }
}

void AbstractVisitor::visit(e9lang::ast::TernaryExpression *e) {
    e->condition->accept(this);
    e->trueExpression->accept(this);
    e->falseExpression->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::ElvisExpression *e) {
    e->condition->accept(this);
    e->value->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::ContainerAccessExpression *e) {
    for(auto &i : e->indexes){
        i->accept(this);
    }
}

void AbstractVisitor::visit(e9lang::ast::ArrayExpression *e) {
    for(auto &i : e->values){
        i->accept(this);
    }
}

void AbstractVisitor::visit(e9lang::ast::MapExpression *e) {
    auto value = e->values.begin();
    auto end = e->keys.end();
    for(auto key = e->keys.begin(); key != end; key++, value++){
        (*key)->accept(this);
        (*value)->accept(this);
    }
}

void AbstractVisitor::visit(e9lang::ast::Argument *a) {
}

void AbstractVisitor::visit(e9lang::ast::Arguments *a) {
    for(auto &i : a->args){
        i->accept(this);
    }
}

void AbstractVisitor::visit(e9lang::ast::IfStatement *s) {
    s->condition->accept(this);
    s->thenStatement->accept(this);
    if(s->elseStatement)
        s->elseStatement->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::VarStatement *s) {
    for(auto &i : s->values){
        i->accept(this);
    }
}

void AbstractVisitor::visit(e9lang::ast::ConstStatement *s) {
    for(auto &i : s->values){
        s->accept(this);
    }
}

void AbstractVisitor::visit(e9lang::ast::PrintStatement *s) {
    s->message->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::FunStatement *s) {
    s->args->accept(this);
    s->body->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::BlockStatement *s) {
    for(auto &i : s->statements){
        i->accept(this);
    }
}

void AbstractVisitor::visit(e9lang::ast::ReturnStatement *s) {
    s->value->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::LoopControlStatement *s) {
}

void AbstractVisitor::visit(e9lang::ast::WhileLoop *s) {
    s->condition->accept(this);
    s->body->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::ForLoop *s) {
    if(s->init)
        s->init->accept(this);
    if(s->condition)
        s->condition->accept(this);
    if(s->update)
        s->update->accept(this);
    s->body->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::EmptyStatement *s) {
}

void AbstractVisitor::visit(e9lang::ast::ExpressionStatement *s) {
    s->expression->accept(this);
}

void AbstractVisitor::visit(e9lang::ast::Unit *unit) {
    for(auto &i : unit->functions){
        i->accept(this);
    }
}
