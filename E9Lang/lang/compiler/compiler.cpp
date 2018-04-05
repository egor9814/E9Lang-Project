//
// Created by egor9814 on 27.03.18.
//

#include <e9lang/compiler.hpp>

e9lang::compiler::CompilerVisitor::~CompilerVisitor() {
    delete writer;
    nullExpr->finalize();
}

void e9lang::compiler::CompilerVisitor::binary(e9lang::parser::Token *operation) {
    switch (operation->type){
        default:
            break;
        case parser::TokenType_PLUS:
            writer->add();
            break;
        case parser::TokenType_MINUS:
            writer->sub();
            break;
        case parser::TokenType_STAR:
            writer->mul();
            break;
        case parser::TokenType_SLASH:
            writer->div();
            break;
        case parser::TokenType_STAR_STAR:
            writer->pow();
            break;
        case parser::TokenType_PERCENT:
            writer->rem();
            break;
        case parser::TokenType_SHIFT_LEFT:
            writer->binaryShiftLeft();
            break;
        case parser::TokenType_U_SHIFT_LEFT:
            writer->binaryShiftLeftUnsigned();
            break;
        case parser::TokenType_SHIFT_RIGHT:
            writer->binaryShiftRight();
            break;
        case parser::TokenType_U_SHIFT_RIGHT:
            writer->binaryShiftRightUnsigned();
            break;
        case parser::TokenType_AND_BINARY:
            writer->binaryAnd();
            break;
        case parser::TokenType_OR_BINARY:
            writer->binaryOr();
            break;
        case parser::TokenType_XOR:
            writer->binaryXor();
            break;
        case parser::TokenType_AND:
            writer->logicalAnd();
            break;
        case parser::TokenType_OR:
            writer->logicalOr();
            break;
        case parser::TokenType_EQ_EQ:
            writer->equals();
            break;
        case parser::TokenType_EXCL_EQ:
            writer->notEquals();
            break;
        case parser::TokenType_LT:
            writer->lessThan();
            break;
        case parser::TokenType_LT_EQ:
            writer->lessThanOrEquals();
            break;
        case parser::TokenType_GT:
            writer->greatThan();
            break;
        case parser::TokenType_GT_EQ:
            writer->greatThanOrEquals();
            break;
    }
}


void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::Node *node) {
    throw "cannot compile abstract node";
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::Unit *unit) {
    for(auto &i : unit->functions){
        i->accept(this);
    }
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::UnaryExpression *e) {
    e->target->accept(this);
    switch (e->operation->type){
        default:
            break;
        case parser::TokenType_MINUS:
            writer->unaryNegate();
            break;
        case parser::TokenType_EXCL:
            writer->unaryNot();
            break;
        case parser::TokenType_TILDE:
            writer->unaryInvert();
            break;
        case parser::TokenType_PLUS_PLUS:
            writer->inc();
            break;
        case parser::TokenType_MINUS_MINUS:
            writer->dec();
            break;
    }
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::BinaryExpression *e) {
    e->right->accept(this);
    e->left->accept(this);
    binary(e->operation);
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::ValueExpression *e) {
    char type;
    switch (e->type){
        default:
            type = 'U';
            break;
        case parser::TokenType_CHAR:
            type = 'c';
            break;
        case parser::TokenType_UCHAR:
            type = 'C';
            break;
        case parser::TokenType_SHORT:
            type = 's';
            break;
        case parser::TokenType_USHORT:
            type = 'S';
            break;
        case parser::TokenType_INT:
            type = 'i';
            break;
        case parser::TokenType_UINT:
            type = 'I';
            break;
        case parser::TokenType_LONG:
            type = 'l';
            break;
        case parser::TokenType_ULONG:
            type = 'L';
            break;
        case parser::TokenType_FLOAT:
            type = 'f';
            break;
        case parser::TokenType_DOUBLE:
            type = 'd';
            break;
        case parser::TokenType_TRUE:
            type = 'z';
            break;
        case parser::TokenType_FALSE:
            type = 'Z';
            break;
        case parser::TokenType_STRING:
            type = 'q';
            break;
        case parser::TokenType_IDENTIFIER:
            type = 'Q';
            break;
        case parser::TokenType_NULL:
            type = 'n';
            break;
    }
    writer->loadConst(e->value, type);
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::VarExpression *e) {
    writer->loadVar(e->name);
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::AssignExpression *e) {
    e->value->accept(this);
    e->target->accept(this);
    binary(e->operation);
    e->target->accept(this);
    writer->store();
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::FunctionCallExpression *e) {
    writer->push();
    writer->array();
    if(!e->args.empty()) for(auto i = e->args.rbegin(); i != e->args.rend(); i++){
        (*i)->accept(this);
    }
    writer->array();
    e->name->accept(this);
    writer->call();
    writer->pop();
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::TernaryExpression *e) {
    auto trueL = label++;
    auto exit = label++;
    e->condition->accept(this);
    writer->jmpIfCondition(trueL);
    writer->popTop();
    e->falseExpression->accept(this);
    writer->jmp(exit);
    writer->label(trueL);
    writer->popTop();
    e->trueExpression->accept(this);
    writer->label(exit);
    label++;
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::ElvisExpression *e) {
    auto exit = label++;
    auto is_null = label++;
    e->condition->accept(this);
    nullExpr->accept(this);
    writer->equals();
    writer->jmpIfCondition(is_null);
    writer->popTop();
    e->condition->accept(this);
    writer->jmp(exit);
    writer->label(is_null);
    writer->popTop();
    e->value->accept(this);
    writer->label(exit);
    label++;
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::ContainerAccessExpression *e) {
    auto index = e->indexes.begin();
    auto end = e->indexes.end();
    (*index)->accept(this);
    for(index++; index != end; index++){
        (*index)->accept(this);
        writer->subscr();
    }
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::ArrayExpression *e) {
    writer->array();
    for(auto i = e->values.rbegin(); i != e->values.rend(); i++){
        (*i)->accept(this);
    }
    writer->array();
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::MapExpression *e) {
    writer->map();
    auto end = e->keys.rend();
    auto value = e->values.rbegin();
    for(auto key = e->keys.rbegin(); key != end; key++, value++){
        (*value)->accept(this);
        (*key)->accept(this);
    }
    writer->map();
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::Argument *a) {
    if(a->var_arg){
        writer->argumentVarArg(a->name);
    } else {
        writer->argument(a->name);
    }
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::Arguments *a) {
    writer->arguments();
    for(auto i = a->args.rbegin(); i != a->args.rend(); i++){
        (*i)->accept(this);
    }
    writer->arguments();
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::IfStatement *s) {
    auto then = label++;
    auto exit = label++;
    s->condition->accept(this);
    writer->jmpIfCondition(then);
    writer->popTop();
    if(s->elseStatement)
        s->elseStatement->accept(this);
    writer->jmp(exit);
    writer->label(then);
    writer->popTop();
    s->thenStatement->accept(this);
    writer->label(exit);
    label++;
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::VarStatement *s) {
    auto end = s->names.rend();
    auto value = s->values.rbegin();
    for(auto name = s->names.rbegin(); name != end; name++){
        (*value)->accept(this);
        writer->storeVar((*name)->text);
    }
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::ConstStatement *s) {
    auto end = s->names.rend();
    auto value = s->values.rbegin();
    for(auto name = s->names.rbegin(); name != end; name++){
        (*value)->accept(this);
        writer->defineConst((*name)->text);
    }
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::PrintStatement *s) {
    s->message->accept(this);
    writer->print();
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::FunStatement *s) {
    if(s->name){
        writer->function(s->name->text);
    } else {
        writer->function();
    }
    s->args->accept(this);
    auto exit = label++;
    labels.push(exit);
    s->body->accept(this);
    writer->label(exit);
    writer->returnValue();
    writer->functionEnd();
    labels.pop();
    label++;
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::BlockStatement *s) {
    /*for(auto i = s->statements.rbegin(); i != s->statements.rend(); i++){
        (*i)->accept(this);
    }*/
    for(auto &i : s->statements){
        i->accept(this);
    }
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::ReturnStatement *s) {
    s->value->accept(this);
    writer->jmp(labels.top());
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::LoopControlStatement *s) {
    auto lastLabel = labels.top();
    if(s->type->type == parser::TokenType_BREAK){
        labels.pop();
        auto gotoLabel = labels.top();
        labels.push(lastLabel);
        writer->jmp(gotoLabel);
    } else {
        writer->jmp(lastLabel);
    }
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::WhileLoop *s) {
    auto start = label++;
    auto exit = label++;
    auto block = label++;
    auto cond = label++;
    labels.push(exit);
    labels.push(start);
    writer->label(start);
    if(s->postCondition){
        writer->loadConst("0", 'i');
        writer->label(block);
        writer->popTop();
        s->body->accept(this);
        s->condition->accept(this);
        writer->jmpIfCondition(block);
        writer->popTop();
        writer->jmp(exit);
    } else {
        writer->label(cond);
        s->condition->accept(this);
        writer->jmpIfCondition(block);
        writer->popTop();
        writer->jmp(exit);
        writer->label(block);
        s->body->accept(this);
        writer->popTop();
        writer->jmp(cond);
    }
    writer->label(exit);

    labels.pop();
    labels.pop();
    label++;
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::ForLoop *s) {
    auto cond = label++;
    auto block = label++;
    auto exit = label++;
    labels.push(exit);
    labels.push(cond);
    if(s->init)
        s->init->accept(this);
    writer->label(cond);
    if(s->condition)
        s->condition->accept(this);
    writer->jmpIfCondition(block);
    writer->popTop();
    writer->jmp(exit);
    writer->label(block);
    if(s->update)
        s->update->accept(this);
    writer->popTop();
    writer->jmp(cond);
    writer->label(exit);

    labels.pop();
    labels.pop();
    label++;
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::EmptyStatement *s) {
}

void e9lang::compiler::CompilerVisitor::visit(e9lang::ast::ExpressionStatement *s) {
    s->expression->accept(this);
}
