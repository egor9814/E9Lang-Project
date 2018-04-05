//
// Created by egor9814 on 30.03.18.
//

#ifndef E9LANG_PROJECT_VM_HPP
#define E9LANG_PROJECT_VM_HPP

#include <e9lang/bytecode.hpp>
#include <vector>
#include <map>

namespace e9lang {

    namespace vm {

        struct Instruction {
            compiler::ByteCode command;
            std::vector<std::string> args;

            explicit Instruction(compiler::ByteCode command)
                    : command(command), args() {}

            Instruction(compiler::ByteCode command, const std::string &arg1)
                    : command(command), args() {
                args.push_back(arg1);
            }

            Instruction(compiler::ByteCode command, const std::string &arg1, const std::string &arg2)
                    : command(command), args() {
                args.push_back(arg1);
                args.push_back(arg2);
            }

            void execute();
        };


        class Function {
        public:
            std::string name;
            std::vector<std::string> args;
            unsigned long requiredArgs;
            std::map<unsigned long, Instruction> instructions;
            std::map<unsigned long, unsigned long> labels;

            Function(const std::string &name, std::vector<std::string> &args, unsigned long requiredArgs);

            void label(unsigned long name);

            Instruction &next();

            unsigned long go(unsigned long label);

            void call();
        };

    }

}

#endif //E9LANG_PROJECT_VM_HPP
