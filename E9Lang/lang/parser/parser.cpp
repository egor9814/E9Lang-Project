//
// Created by egor9814 on 15.03.18.
//

#include "parser.hpp"
#include "util.hpp"

e9lang::parser::Unit::Unit(std::list<e9lang::ast::FunStatement *> &functions)
        : functions(functions) {}

std::string e9lang::parser::Unit::toString() {
    std::stringstream ss;
    for (auto &i : functions) {
        ss << "\n";
        ss << i->toString();
    }
    return ss.str();
}

void e9lang::parser::Unit::finalize() {
    while (!functions.empty()) {
        auto f = functions.front();
        functions.pop_front();
        f->finalize();
    }
    delete this;
}


class ParseError {
};

void e9lang::parser::Parser::error(std::string text, e9lang::parser::Token *begin, e9lang::parser::Token *end) {
    errors.push_back(new Token(
            TokenType_ERROR,
            {begin->range.begin, end->range.end},
            begin->begin, end->end,
            "", std::move(text)
    ));
    throw ParseError();
}

e9lang::parser::Parser::Parser(std::list<e9lang::parser::Token *> &tokens) {
    for(auto &i : tokens){
        this->tokens.push_back(i);
    }

    eofToken = new Token(TokenType_EOF, {0, 0}, {0, 0}, {0, 0}, "End of file");

    assignOperators.emplace(TokenType_EQ);
    assignOperators.emplace(TokenType_HOOK_EQ);
    assignOperators.emplace(TokenType_PLUS_EQ);
    assignOperators.emplace(TokenType_MINUS_EQ);
    assignOperators.emplace(TokenType_STAR_STAR_EQ);
    assignOperators.emplace(TokenType_STAR_EQ);
    assignOperators.emplace(TokenType_SLASH_EQ);
    assignOperators.emplace(TokenType_PERCENT_EQ);
    assignOperators.emplace(TokenType_AND_BINARY_EQ);
    assignOperators.emplace(TokenType_OR_BINARY_EQ);
    assignOperators.emplace(TokenType_XOR_EQ);
    assignOperators.emplace(TokenType_SHIFT_LEFT_EQ);
    assignOperators.emplace(TokenType_SHIFT_RIGHT_EQ);
    assignOperators.emplace(TokenType_U_SHIFT_LEFT_EQ);
    assignOperators.emplace(TokenType_U_SHIFT_RIGHT_EQ);
}

e9lang::parser::Parser::~Parser() {
    e9lang::util::deallocate(errors);
    delete eofToken;
    if(parsedUnit)
        parsedUnit->finalize();
}


e9lang::parser::Token *e9lang::parser::Parser::get(unsigned long relPos) {
    /*if (relPos > 0) {
        for (auto i = 0ul; i < relPos && current != last; i++)
            current++;
    } else if (relPos < 0) {
        for (auto i = 0ul; i < -relPos && i < tokens.size(); i++)
            current--;
    }
    auto result = current == last ? eofToken : *current;
    if (relPos > 0) {
        for (auto i = 0ul; i < relPos && current != last; i++)
            current--;
    } else if (relPos < 0) {
        for (auto i = 0ul; i < -relPos && i < tokens.size(); i++)
            current++;
    }
    return result;*/
    auto p = pos + relPos;
    if(p >= tokens.size())
        return eofToken;
    return tokens[p];
}

e9lang::parser::Token *e9lang::parser::Parser::require(e9lang::parser::TokenType type) {
    auto next = get();
    if (next->type != type) {
        error("expected '" + tokenNameOf(type) + "', but found '" + next->getType() + "'", next, next);
    } else {
        //current++;
        pos++;
        return next;
    }
}

bool e9lang::parser::Parser::match(e9lang::parser::TokenType type) {
    auto next = get();
    if (next->type != type)
        return false;
    //current++;
    pos++;
    return true;
}

bool e9lang::parser::Parser::lookMatch(unsigned long relPos, e9lang::parser::TokenType type) {
    return get(relPos)->type == type;
}


e9lang::ast::Expression *e9lang::parser::Parser::value() {
    auto current = get();
    switch (current->type) {
        default:
            error("unknown expression: " + current->getType(), current, current);
            break;
        case TokenType_CHAR:
        case TokenType_UCHAR:
        case TokenType_SHORT:
        case TokenType_USHORT:
        case TokenType_INT:
        case TokenType_UINT:
        case TokenType_LONG:
        case TokenType_ULONG:
        case TokenType_FLOAT:
        case TokenType_DOUBLE:
        case TokenType_TRUE:
        case TokenType_FALSE:
        case TokenType_STRING:
        case TokenType_NULL:
            match(current->type);
            return new ast::ValueExpression(current);
    }
}

std::list<e9lang::ast::Expression *> e9lang::parser::Parser::qualifier() {
    std::list<ast::Expression *> indexes;
    if (lookMatch(0, TokenType_DOT) || lookMatch(0, TokenType_LBRACKET)) {
        while (lookMatch(0, TokenType_DOT) || lookMatch(0, TokenType_LBRACKET)) {
            if (match(TokenType_DOT)) {
                indexes.push_back(new ast::ValueExpression(require(TokenType_IDENTIFIER)));
            }
            if (match(TokenType_LBRACKET)) {
                indexes.push_back(expression());
                require(TokenType_RBRACKET);
            }
        }
    }
    return indexes;
}

e9lang::ast::Expression *e9lang::parser::Parser::qualifiedName() {
    auto current = get();
    if (!match(TokenType_IDENTIFIER))
        return nullptr;
    auto indexes = qualifier();
    if (indexes.empty())
        return new ast::VarExpression(current);
    return new ast::ContainerAccessExpression(indexes);
}

e9lang::ast::Expression *e9lang::parser::Parser::variable() {
    if (lookMatch(0, TokenType_IDENTIFIER) && lookMatch(1, TokenType_LPAREN)) {
        return functionCalls(new ast::ValueExpression(require(TokenType_IDENTIFIER)));
    }

    auto var = qualifiedName();
    if (var) {
        if (lookMatch(0, TokenType_LPAREN)) {
            return functionCalls(new ast::ValueExpression(require(TokenType_IDENTIFIER)));
        }
        auto current = get();
        if (match(TokenType_PLUS_PLUS)) {
            return new ast::UnaryExpression(var, current);
        }
        if (match(TokenType_MINUS_MINUS)) {
            return new ast::UnaryExpression(var, current);
        }
        return var;
    }

    return value();
}

e9lang::ast::Expression *e9lang::parser::Parser::primary() {
    if (match(TokenType_LPAREN)) {
        auto e = expression();
        match(TokenType_RPAREN);
        return e;
    }

    if (match(TokenType_FUN)) {
        //TODO: implement lambda-function definition
        return nullptr;
    }

    return variable();
}

e9lang::ast::Expression *e9lang::parser::Parser::unary() {
    auto current = get(0);
    if (match(TokenType_MINUS) || match(TokenType_EXCL) || match(TokenType_TILDE))
        return new ast::UnaryExpression(primary(), current);
    match(TokenType_PLUS);
    return primary();
}

e9lang::ast::Expression *e9lang::parser::Parser::multiplicative() {
    auto result = unary();
    while (true) {
        auto current = get();
        if (match(TokenType_STAR) || match(TokenType_SLASH)
            || match(TokenType_STAR_STAR) || match(TokenType_PERCENT)) {
            result = new ast::BinaryExpression(result, unary(), current);
        } else break;
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::additive() {
    auto result = multiplicative();
    while (true) {
        auto current = get();
        if (match(TokenType_PLUS) || match(TokenType_MINUS)) {
            result = new ast::BinaryExpression(result, multiplicative(), current);
        } else break;
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::shift() {
    auto result = additive();
    while (true) {
        auto current = get();
        if (match(TokenType_SHIFT_LEFT) || match(TokenType_SHIFT_RIGHT)
            || match(TokenType_U_SHIFT_LEFT) || match(TokenType_U_SHIFT_RIGHT)) {
            result = new ast::BinaryExpression(result, additive(), current);
        } else break;
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::conditional() {
    auto result = shift();
    while (true) {
        auto current = get();
        if (match(TokenType_LT) || match(TokenType_GT)
            || match(TokenType_LT_EQ) || match(TokenType_GT_EQ)) {
            result = new ast::BinaryExpression(result, shift(), current);
        } else break;
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::equality() {
    auto result = conditional();
    auto current = get();
    if (match(TokenType_EQ_EQ) || match(TokenType_EXCL_EQ)) {
        return new ast::BinaryExpression(result, conditional(), current);
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::bitwiseAnd() {
    auto result = equality();
    while (true) {
        auto current = get();
        if (match(TokenType_AND_BINARY)) {
            result = new ast::BinaryExpression(result, equality(), current);
        } else break;
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::bitwiseXor() {
    auto result = bitwiseAnd();
    while (true) {
        auto current = get();
        if (match(TokenType_XOR)) {
            result = new ast::BinaryExpression(result, bitwiseAnd(), current);
        } else break;
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::bitwiseOr() {
    auto result = bitwiseXor();
    while (true) {
        auto current = get();
        if (match(TokenType_OR_BINARY)) {
            result = new ast::BinaryExpression(result, bitwiseXor(), current);
        } else break;
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::logicalAnd() {
    auto result = bitwiseOr();
    while (true) {
        auto current = get();
        if (match(TokenType_AND)) {
            result = new ast::BinaryExpression(result, bitwiseOr(), current);
        } else break;
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::logicalOr() {
    auto result = logicalAnd();
    while (true) {
        auto current = get();
        if (match(TokenType_OR)) {
            result = new ast::BinaryExpression(result, logicalAnd(), current);
        } else break;
    }
    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::ternary() {
    auto result = logicalOr();

    if (match(TokenType_HOOK)) {
        auto trueExpr = expression();
        require(TokenType_COLON);
        auto falseExpr = expression();
        return new ast::TernaryExpression(result, trueExpr, falseExpr);
    }

    if (match(TokenType_HOOK_COLON)) {
        return new ast::ElvisExpression(result, expression());
    }

    return result;
}

e9lang::ast::Expression *e9lang::parser::Parser::assignmentStrict() {
    auto recover = [&](unsigned long save_pos) -> ast::Expression * {
        /*auto diff = pos - save_pos;
        for (auto i = 0ul; i < diff; i++) {
            current--;
        }*/
        pos = save_pos;
        return nullptr;
    };

    auto save_pos = pos;
    auto target = qualifiedName();
    if (!target || !target->isAccessible()) {
        return recover(save_pos);
    }

    auto current = get();
    if (assignOperators.count(current->type) == 0) {
        return recover(save_pos);
    }
    match(current->type);

    return new ast::AssignExpression(reinterpret_cast<ast::AccessibleExpression *>(target), expression(), current);
}

e9lang::ast::Expression *e9lang::parser::Parser::assignment() {
    auto assign = assignmentStrict();
    if (assign)
        return assign;
    return ternary();
}

e9lang::ast::Expression *e9lang::parser::Parser::expression() {
    return assignment();
}

e9lang::ast::FunctionCallExpression *e9lang::parser::Parser::functionCall(e9lang::ast::Expression *name) {
    require(TokenType_LPAREN);
    auto f = new ast::FunctionCallExpression(name);
    while (!match(TokenType_RPAREN)) {
        f->addArgument(expression());
        match(TokenType_COMMA);
    }
    return f;
}

e9lang::ast::Expression *e9lang::parser::Parser::functionCalls(e9lang::ast::Expression *name) {
    auto f = functionCall(name);
    if (lookMatch(0, TokenType_LPAREN)) {
        return functionCalls(f);
    }
    if (lookMatch(0, TokenType_DOT)) {
        auto indexes = qualifier();
        if (indexes.empty())
            return f;
        if (lookMatch(0, TokenType_LPAREN)) {
            return functionCalls(new ast::ContainerAccessExpression(f, indexes));
        }
        return new ast::ContainerAccessExpression(f, indexes);
    }
    return f;
}

e9lang::ast::Arguments *e9lang::parser::Parser::arguments() {
    auto args = new ast::Arguments;
    require(TokenType_LPAREN);
    while (!match(TokenType_RPAREN)) {
        auto name = require(TokenType_IDENTIFIER);
        bool var_arg = false;
        if (match(TokenType_LBRACKET)) {
            require(TokenType_RBRACKET);
            var_arg = true;
        }
        args->add(new ast::Argument(name, var_arg));
        match(TokenType_COMMA);
    }
    return args;
}

e9lang::ast::FunStatement *e9lang::parser::Parser::functionDefine() {
    auto start = get();
    auto name = require(TokenType_IDENTIFIER);
    auto args = arguments();
    ast::Statement *body = nullptr;
    if (match(TokenType_EQ)) {
        body = new ast::ReturnStatement(expression());
    } else if(match(TokenType_LBRACE)){
        body = statementBlock();
    } else {
        auto current = get();
        error("unknown function define type: " + current->text, start, current);
    }
    return new ast::FunStatement(name, args, body);
}


e9lang::ast::Statement *e9lang::parser::Parser::statementBlock() {
    auto block = new ast::BlockStatement;
    while (!match(TokenType_RBRACE)) {
        block->add(statementOrBlock());
    }
    return block;
}

e9lang::ast::Statement *e9lang::parser::Parser::statementOrBlock() {
    if (match(TokenType_LBRACE)) {
        return statementBlock();
    } else {
        return statement();
    }
}

e9lang::ast::IfStatement *e9lang::parser::Parser::ifStatement() {
    require(TokenType_LPAREN);
    auto condition = expression();
    require(TokenType_RPAREN);
    auto then = statementOrBlock();
    auto els = match(TokenType_ELSE) ? statementOrBlock() : nullptr;
    return new ast::IfStatement(condition, then, els);
}

void e9lang::parser::Parser::varOrConstDeclaration(std::list<Token *> &names, std::list<ast::Expression *> &values) {
    do {
        names.push_back(require(TokenType_IDENTIFIER));
        if (match(TokenType_EQ)) {
            values.push_back(expression());
        } else {
            values.push_back(nullptr);
        }
    } while (match(TokenType_COMMA));
}

e9lang::ast::VarStatement *e9lang::parser::Parser::varDeclaration() {
    std::list<Token *> names;
    std::list<ast::Expression *> values;
    varOrConstDeclaration(names, values);
    return new ast::VarStatement(names, values);
}

e9lang::ast::ConstStatement *e9lang::parser::Parser::constDeclaration() {
    std::list<Token *> names;
    std::list<ast::Expression *> values;
    varOrConstDeclaration(names, values);
    return new ast::ConstStatement(names, values);
}

e9lang::ast::PrintStatement *e9lang::parser::Parser::print() {
    return new ast::PrintStatement(expression());
}

e9lang::ast::ReturnStatement *e9lang::parser::Parser::returnStatement() {
    return new ast::ReturnStatement(expression());
}

e9lang::ast::ForLoop *e9lang::parser::Parser::forLoop() {
    require(TokenType_LPAREN);
    ast::Statement *init, *update, *body;
    ast::Expression *condition;
    init = match(TokenType_SEMICOLON) ? nullptr : statementOrBlock();
    condition = match(TokenType_SEMICOLON) ? nullptr : expression();
    update = lookMatch(0, TokenType_RPAREN) ? nullptr : statementOrBlock();
    require(TokenType_RPAREN);
    body = statementOrBlock();
    return new ast::ForLoop(init, condition, update, body);
}

e9lang::ast::WhileLoop *e9lang::parser::Parser::whileLoop() {
    require(TokenType_LPAREN);
    auto condition = expression();
    require(TokenType_RPAREN);
    auto body = statementOrBlock();
    return new ast::WhileLoop(false, condition, body);
}

e9lang::ast::WhileLoop *e9lang::parser::Parser::doWhileLoop() {
    require(TokenType_LBRACE);
    auto body = statementBlock();
    require(TokenType_WHILE);
    require(TokenType_LPAREN);
    auto condition = expression();
    require(TokenType_RPAREN);
    return new ast::WhileLoop(true, condition, body);
}

e9lang::ast::Statement *e9lang::parser::Parser::statement() {
    try {
        if(match(TokenType_SEMICOLON))
            return new ast::EmptyStatement;

        if(match(TokenType_RETURN))
            return returnStatement();

        if(match(TokenType_IF))
            return ifStatement();

        if(match(TokenType_VAR))
            return varDeclaration();

        if(match(TokenType_CONST))
            return constDeclaration();

        if(match(TokenType_PRINT))
            return print();

        if(match(TokenType_FOR))
            return forLoop();

        if(match(TokenType_WHILE))
            return whileLoop();

        if(match(TokenType_DO))
            return doWhileLoop();

        auto current = get();
        if(match(TokenType_BREAK) || match(TokenType_CONTINUE))
            return new ast::LoopControlStatement(current);

        auto assign = assignmentStrict();
        if(assign)
            return new ast::ExpressionStatement(assign);

        error("unknown statement: " + current->text, current, current);
    } catch (ParseError& err){
        auto current = get();
        pos++;
        while(!match(TokenType_EOF) && get()->begin.row == current->end.row)
            pos++;
        if(get()->type != TokenType_EOF)
            return statement();
    }
    return nullptr;
}


e9lang::parser::Unit *e9lang::parser::Parser::getUnit() const {
    return parsedUnit;
}


bool e9lang::parser::Parser::parse() {
    if(parsedUnit)
        parsedUnit->finalize();
    pos = 0;
    util::deallocate(errors);

    std::list<ast::FunStatement*> functions;
    while(!match(TokenType_EOF)){
        try {
            if(match(TokenType_FUN))
                functions.push_back(functionDefine());
        } catch (ParseError& err){
            while(!lookMatch(0, TokenType_FUN) && !lookMatch(0, TokenType_EOF)){
                pos++;
            }
        }
    }
    parsedUnit = new Unit(functions);

    return errors.empty();
}

e9lang::parser::Parser::TokenList e9lang::parser::Parser::getErrors() const {
    return errors;
}
