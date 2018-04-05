//
// Created by egor9814 on 27.03.18.
//

#include <e9lang/compiler.hpp>
#include <iostream>
#include "e9lang/lexer.hpp"
#include "e9lang/parser.hpp"

int TestCompile() {
    using namespace e9lang::parser;
    FileSourceProvider sourceProvider("../tests/test2.enl");
    if (!sourceProvider.open()) {
        std::cerr << "cannot open input file" << std::endl;
        return -1;
    }
    LexerConfig config;
    Lexer lexer(config, sourceProvider);
    sourceProvider.close();

    if (lexer.tokenize()) {
        auto tokens = lexer.getTokens();
        /*for(auto &i : tokens) {
            std::cout << i->toString() << std::endl;
        }*/
        Parser parser(tokens);
        if (parser.parse()) {
            auto unit = parser.getUnit();
            std::cout << unit->toString() << std::endl;

            std::fstream out("test2.cenl", std::ios_base::out | std::ios_base::binary);
            /*Token nullToken(TokenType_NULL, {0, 0}, {0, 0}, {0, 0}, "null", "");
            e9lang::compiler::CompilerVisitor visitor(out, &nullToken);
            visitor.visit(unit);*/
            e9lang::compiler::compile(out, unit);
            out.close();
            return 0;
        } else {
            for (auto &i : parser.getErrors()) {
                std::cerr << i->toString() << std::endl;
            }
        }
    } else {
        for (auto &i : lexer.getWarnings()) {
            std::cout << i->toString() << std::endl;
        }
        for (auto &i : lexer.getErrors()) {
            std::cerr << i->toString() << std::endl;
        }
    }
    return -1;
}

inline std::fstream &operator^=(std::fstream &input, std::string &output) {
    std::stringstream ss;
    char c;
    for (input >> c; c != 0; input >> c) {
        ss << (char)~c;
    }
    output += ss.str();
    return input;
}

int TestDecompile() {
    using namespace std;
    using namespace e9lang::compiler;
    fstream in("test2.cenl", ios_base::in | ios_base::binary);
    if (!in.is_open()) {
        cerr << "cannot open file" << endl;
        return -1;
    }
    in.unsetf(ios_base::skipws);
    cout << endl << "DECOMPILED:" << endl;

    char cmd;
    string tmp;
    char tmp2;
    unsigned long tmp3;
    for (in >> cmd; !in.eof(); in >> cmd) {
        switch ((ByteCode) cmd) {
            default:
                cout << "          (unknown)" << endl;
                break;
            case ByteCode_NOP:
                break;
            case ByteCode_PUSH:
                cout << "          PUSH" << endl;
                break;
            case ByteCode_POP:
                cout << "          POP" << endl;
                break;
            case ByteCode_POP_TOP:
                cout << "          POP_TOP" << endl;
                break;

            case ByteCode_STORE:
                cout << "          STORE";
                break;
            case ByteCode_STORE_VAR:
                cout << "          STORE_VAR ";
                tmp.clear();
                ::operator^=(in, tmp);
                cout << tmp << endl;
                break;
            case ByteCode_DEFINE_CONST:
                cout << "          DEFINE_CONST ";
                tmp.clear();
                ::operator^=(in, tmp);
                cout << tmp << endl;
                break;
            case ByteCode_LOAD_VAR:
                cout << "          LOAD_VAR ";
                tmp.clear();
                ::operator^=(in, tmp);
                cout << tmp << endl;
                break;
            case ByteCode_LOAD_CONST:
                cout << "          LOAD_CONST ";
                tmp.clear();
                ::operator^=(in, tmp);
                in >> tmp2;
                switch (tmp2) {
                    default:
                        cout << tmp << tmp2 << endl;
                        break;
                    case 'q':
                        cout << '"' << tmp << '"' << endl;
                        break;
                    case 'Q':
                    case 'z':
                    case 'Z':
                    case 'n':
                        cout << tmp << endl;
                        break;
                    case 'U':
                        cout << "<invalid type>" << endl;
                        break;
                }
                break;

            case ByteCode_UNARY_NEGATE:
                cout << "          UNARY_NEGATE (-)" << endl;
                break;
            case ByteCode_UNARY_NOT:
                cout << "          UNARY_NOT (!)" << endl;
                break;
            case ByteCode_UNARY_INVERT:
                cout << "          UNARY_INVERT (~)" << endl;
                break;

            case ByteCode_BINARY_AND:
                cout << "          BINARY_AND (&)" << endl;
                break;
            case ByteCode_BINARY_OR:
                cout << "          BINARY_OR (|)" << endl;
                break;
            case ByteCode_BINARY_XOR:
                cout << "          BINARY_XOR (^)" << endl;
                break;
            case ByteCode_BINARY_SHIFT_RIGHT:
                cout << "          BINARY_SHIFT_RIGHT (>>)" << endl;
                break;
            case ByteCode_BINARY_SHIFT_RIGHT_UNSIGNED:
                cout << "          BINARY_SHIFT_RIGHT_UNSIGNED (>>>)" << endl;
                break;
            case ByteCode_BINARY_SHIFT_LEFT:
                cout << "          BINARY_SHIFT_LEFT (<<)" << endl;
                break;
            case ByteCode_BINARY_SHIFT_LEFT_UNSIGNED:
                cout << "          BINARY_SHIFT_LEFT_UNSIGNED (<<<)" << endl;
                break;

            case ByteCode_AND:
                cout << "          LOGICAL_AND (and)" << endl;
                break;
            case ByteCode_OR:
                cout << "          LOGICAL_OR (or)" << endl;
                break;

            case ByteCode_EQUALS:
                cout << "          EQUALS (==)" << endl;
                break;
            case ByteCode_NOT_EQUALS:
                cout << "          NOT_EQUALS (!=)" << endl;
                break;
            case ByteCode_GT:
                cout << "          GT (>)" << endl;
                break;
            case ByteCode_GT_EQUALS:
                cout << "          GT_EQ (>=)" << endl;
                break;
            case ByteCode_LT:
                cout << "          LT (<)" << endl;
                break;
            case ByteCode_LT_EQUALS:
                cout << "          LT_EQ (<=)" << endl;
                break;

            case ByteCode_SUBSCR:
                cout << "          SUBSCR ([])" << endl;
                break;
            case ByteCode_CALL:
                cout << "          CALL (())" << endl;
                break;

            case ByteCode_ADD:
                cout << "          ADD (+)" << endl;
                break;
            case ByteCode_SUB:
                cout << "          SUB (-)" << endl;
                break;
            case ByteCode_MUL:
                cout << "          MUL (*)" << endl;
                break;
            case ByteCode_DIV:
                cout << "          DIV (/)" << endl;
                break;
            case ByteCode_REM:
                cout << "          REM (%)" << endl;
                break;
            case ByteCode_POW:
                cout << "          POW (**)" << endl;
                break;

            case ByteCode_INC:
                cout << "          INC (++)" << endl;
                break;
            case ByteCode_DEC:
                cout << "          DEC (--)" << endl;
                break;

            case ByteCode_MAP:
                cout << "          MAP" << endl;
                break;
            case ByteCode_ARRAY:
                cout << "          ARRAY" << endl;
                break;

            case ByteCode_JMP:
                cout << "          JMP ";
                tmp.clear();
                ::operator^=(in, tmp);
                tmp3 = (unsigned)std::stol(tmp);
                printf("0x%08lx\n", tmp3);
                break;
            case ByteCode_JMP_IF:
                cout << "          JMP_IF ";
                tmp.clear();
                ::operator^=(in, tmp);
                tmp3 = (unsigned)std::stol(tmp);
                printf("0x%08lx\n", tmp3);
                break;

            case ByteCode_LABEL:
                tmp.clear();
                ::operator^=(in, tmp);
                tmp3 = (unsigned)std::stol(tmp);
                printf("0x%08lx:\n", tmp3);
                break;

            case ByteCode_FUNCTION:
                cout << "FUNCTION_DEFINE ";
                tmp.clear();
                ::operator^=(in, tmp);
                cout << tmp << endl;
                break;
            case ByteCode_FUNCTION_END:
                cout << "FUNCTION_END" << endl;
                break;
            case ByteCode_FUNCTION_WITHOUT_NAME:
                cout << "FUNCTION_DEFINE" << endl;
                break;
            case ByteCode_ARGUMENTS:
                cout << "          FUNCTION_ARGUMENTS" << endl;
                break;
            case ByteCode_ARGUMENT:
                cout << "          FUNCTION_ARGUMENT ";
                tmp.clear();
                ::operator^=(in, tmp);
                cout << tmp << endl;
                break;
            case ByteCode_ARGUMENT_VARARG:
                cout << "          FUNCTION_ARGUMENT ";
                tmp.clear();
                ::operator^=(in, tmp);
                cout << tmp << "[]" << endl;
                break;
            case ByteCode_RETURN:
                cout << "          RETURN_VALUE" << endl;
                break;

            case ByteCode_PRINT:
                cout << "          PRINT" << endl;
                break;
        }
    }
    in.close();
    return 0;
}

int main() {
    auto res = TestCompile();
    if (res)
        return res;
    return TestDecompile();
}