//
// Created by egor9814 on 27.03.18.
//

#ifndef E9LANG_PROJECT_COMPILER_HPP
#define E9LANG_PROJECT_COMPILER_HPP

#include <stack>
#include "ast.hpp"
#include "bytecode.hpp"

namespace e9lang {

    namespace compiler {

        class ByteCodeWriter {
        public:
            virtual void push() = 0;

            virtual void pop() = 0;

            virtual void popTop() = 0;


            virtual void store() = 0;

            virtual void storeVar(const std::string &name) = 0;

            virtual void defineConst(const std::string &name) = 0;

            virtual void loadConst(const std::string &value, char type) = 0;

            virtual void loadVar(const std::string &name) = 0;


            virtual void unaryNegate() = 0;

            virtual void unaryNot() = 0;

            virtual void unaryInvert() = 0;


            virtual void binaryAnd() = 0;

            virtual void binaryOr() = 0;

            virtual void binaryXor() = 0;


            virtual void binaryShiftLeft() = 0;

            virtual void binaryShiftLeftUnsigned() = 0;

            virtual void binaryShiftRight() = 0;

            virtual void binaryShiftRightUnsigned() = 0;


            virtual void logicalAnd() = 0;

            virtual void logicalOr() = 0;


            virtual void equals() = 0;

            virtual void notEquals() = 0;

            virtual void lessThan() = 0;

            virtual void lessThanOrEquals() = 0;

            virtual void greatThan() = 0;

            virtual void greatThanOrEquals() = 0;


            virtual void subscr() = 0;

            virtual void call() = 0;


            virtual void add() = 0;

            virtual void sub() = 0;

            virtual void mul() = 0;

            virtual void div() = 0;

            virtual void rem() = 0;

            virtual void pow() = 0;


            virtual void inc() = 0;

            virtual void dec() = 0;


            virtual void map() = 0;

            virtual void array() = 0;


            virtual void jmp(unsigned long label) = 0;

            virtual void jmpIfCondition(unsigned long label) = 0;


            virtual void label(unsigned long label) = 0;


            virtual void function(const std::string &name) = 0;

            virtual void function() = 0;

            virtual void functionEnd() = 0;

            virtual void arguments() = 0;

            virtual void argument(const std::string &name) = 0;

            virtual void argumentVarArg(const std::string &name) = 0;

            virtual void returnValue() = 0;


            virtual void print() = 0;
        };

        template<typename OutputStream>
        class StreamByteCodeWriter : public ByteCodeWriter {
            OutputStream &out;

            void cmd(ByteCode bc) {
                out << (char) bc;
            }

            void str(const std::string &val) {
                for (auto &i : val) {
                    out << (char)~i;
                }
                out << '\0';
            }

        public:
            explicit StreamByteCodeWriter(OutputStream &outputStream)
                    : out(outputStream) {}

            void push() override {
                cmd(ByteCode_PUSH);
            }

            void pop() override {
                cmd(ByteCode_POP);
            }

            void popTop() override {
                cmd(ByteCode_POP_TOP);
            }


            void store() override {
                cmd(ByteCode_STORE);
            }

            void storeVar(const std::string &name) override {
                cmd(ByteCode_STORE_VAR);
                str(name);
            }

            void defineConst(const std::string &name) override {
                cmd(ByteCode_DEFINE_CONST);
                str(name);
            }

            void loadConst(const std::string &value, char type) override {
                cmd(ByteCode_LOAD_CONST);
                str(value);
                out << type;
            }

            void loadVar(const std::string &name) override {
                cmd(ByteCode_LOAD_VAR);
                str(name);
            }


            void unaryNegate() override {
                cmd(ByteCode_UNARY_NEGATE);
            }

            void unaryNot() override {
                cmd(ByteCode_UNARY_NOT);
            }

            void unaryInvert() override {
                cmd(ByteCode_UNARY_INVERT);
            }


            void binaryAnd() override {
                cmd(ByteCode_BINARY_AND);
            }

            void binaryOr() override {
                cmd(ByteCode_BINARY_OR);
            }

            void binaryXor() override {
                cmd(ByteCode_BINARY_XOR);
            }


            void binaryShiftLeft() override {
                cmd(ByteCode_BINARY_SHIFT_LEFT);
            }

            void binaryShiftLeftUnsigned() override {
                cmd(ByteCode_BINARY_SHIFT_LEFT_UNSIGNED);
            }

            void binaryShiftRight() override {
                cmd(ByteCode_BINARY_SHIFT_RIGHT);
            }

            void binaryShiftRightUnsigned() override {
                cmd(ByteCode_BINARY_SHIFT_RIGHT_UNSIGNED);
            }


            void logicalAnd() override {
                cmd(ByteCode_AND);
            }

            void logicalOr() override {
                cmd(ByteCode_OR);
            }


            void equals() override {
                cmd(ByteCode_EQUALS);
            }

            void notEquals() override {
                cmd(ByteCode_NOT_EQUALS);
            }

            void lessThan() override {
                cmd(ByteCode_LT);
            }

            void lessThanOrEquals() override {
                cmd(ByteCode_LT_EQUALS);
            }

            void greatThan() override {
                cmd(ByteCode_GT);
            }

            void greatThanOrEquals() override {
                cmd(ByteCode_GT_EQUALS);
            }


            void subscr() override {
                cmd(ByteCode_SUBSCR);
            }

            void call() override {
                cmd(ByteCode_CALL);
            }


            void add() override {
                cmd(ByteCode_ADD);
            }

            void sub() override {
                cmd(ByteCode_SUB);
            }

            void mul() override {
                cmd(ByteCode_MUL);
            }

            void div() override {
                cmd(ByteCode_DIV);
            }

            void rem() override {
                cmd(ByteCode_REM);
            }

            void pow() override {
                cmd(ByteCode_POW);
            }


            void inc() override {
                cmd(ByteCode_INC);
            }

            void dec() override {
                cmd(ByteCode_DEC);
            }


            void map() override {
                cmd(ByteCode_MAP);
            }

            void array() override {
                cmd(ByteCode_ARRAY);
            }


            void jmp(unsigned long label) override {
                cmd(ByteCode_JMP);
                str(std::to_string((signed)label));
            }

            void jmpIfCondition(unsigned long label) override {
                cmd(ByteCode_JMP_IF);
                str(std::to_string((signed)label));
            }


            void label(unsigned long label) override {
                cmd(ByteCode_LABEL);
                str(std::to_string((signed)label));
            }


            void function(const std::string &name) override {
                cmd(ByteCode_FUNCTION);
                str(name);
            }

            void function() override {
                cmd(ByteCode_FUNCTION_WITHOUT_NAME);
            }

            void functionEnd() override {
                cmd(ByteCode_FUNCTION_END);
            }

            void arguments() override {
                cmd(ByteCode_ARGUMENTS);
            }

            void argument(const std::string &name) override {
                cmd(ByteCode_ARGUMENT);
                str(name);
            }

            void argumentVarArg(const std::string &name) override {
                cmd(ByteCode_ARGUMENT_VARARG);
                str(name);
            }

            void returnValue() override {
                cmd(ByteCode_RETURN);
            }


            void print() override {
                cmd(ByteCode_PRINT);
            }
        };


        class CompilerVisitor : public ast::Visitor {
            ByteCodeWriter *writer = nullptr;

            void binary(parser::Token *operation);

            unsigned long label = 1;

            ast::ValueExpression *nullExpr = nullptr;

            std::stack<unsigned long> labels{};
        public:
            template<typename OutputStream>
            CompilerVisitor(OutputStream &out, parser::Token *nullToken)
                    : writer(new StreamByteCodeWriter<OutputStream>(out)),
                      nullExpr(new ast::ValueExpression(nullToken)) {}

            ~CompilerVisitor();

            void visit(ast::Node *node) override;

            void visit(ast::Unit *unit) override;

            void visit(ast::UnaryExpression *e) override;

            void visit(ast::BinaryExpression *e) override;

            void visit(ast::ValueExpression *e) override;

            void visit(ast::VarExpression *e) override;

            void visit(ast::AssignExpression *e) override;

            void visit(ast::FunctionCallExpression *e) override;

            void visit(ast::TernaryExpression *e) override;

            void visit(ast::ElvisExpression *e) override;

            void visit(ast::ContainerAccessExpression *e) override;

            void visit(ast::ArrayExpression *e) override;

            void visit(ast::MapExpression *e) override;

            void visit(ast::Argument *a) override;

            void visit(ast::Arguments *a) override;

            void visit(ast::IfStatement *s) override;

            void visit(ast::VarStatement *s) override;

            void visit(ast::ConstStatement *s) override;

            void visit(ast::PrintStatement *s) override;

            void visit(ast::FunStatement *s) override;

            void visit(ast::BlockStatement *s) override;

            void visit(ast::ReturnStatement *s) override;

            void visit(ast::LoopControlStatement *s) override;

            void visit(ast::WhileLoop *s) override;

            void visit(ast::ForLoop *s) override;

            void visit(ast::EmptyStatement *s) override;

            void visit(ast::ExpressionStatement *s) override;
        };


        template<typename OutputStream>
        inline void compile(OutputStream &outputStream, ast::Unit *unit) {
            parser::Token nul(parser::TokenType_NULL, {0, 0}, {0, 0}, {0, 0}, "null");
            CompilerVisitor visitor(outputStream, &nul);
            unit->accept(&visitor);
        }
    }

}

#endif //E9LANG_PROJECT_COMPILER_HPP
