//
// Created by egor9814 on 30.03.18.
//

#ifndef E9LANG_PROJECT_OBJECTS_HPP
#define E9LANG_PROJECT_OBJECTS_HPP

#include <vector>
#include <functional>
#include <map>
#include "stack.hpp"

namespace e9lang {

    namespace objects {

        enum ObjectType {
            ObjectType_NULL,
            ObjectType_BOOL,
            ObjectType_CHAR,
            ObjectType_INT,
            ObjectType_FLOAT,
            ObjectType_STRING,
            ObjectType_ARRAY,
            ObjectType_MAP,
            ObjectType_FUNCTION,
        };


        class Object;

        class Null;

        class Boolean;

        class Char;

        class Int;

        class Float;

        class String;

        class Array;

        class Map;

        class Function;


        typedef std::function<Object *(vm::Stack *, std::vector<Object *> &args)> FunctionType;


        class Object {
            bool constant = false;
        public:
            Object();

            ~Object();

            virtual unsigned long hashCode() = 0;

            virtual ObjectType getType() = 0;

#define DEFINE_CAST(Type, Name) virtual Type* Name(vm::Stack *s = nullptr) { return nullptr; }

            DEFINE_CAST(Null, toNull)

            DEFINE_CAST(Char, toChar)

            DEFINE_CAST(Int, toInt)

            DEFINE_CAST(Float, toFloat)

            DEFINE_CAST(Array, toArray)

            DEFINE_CAST(Map, toMap)

            DEFINE_CAST(Function, toFunction)

#undef DEFINE_CAST


            //std::map<std::string, Function> functions, operators;
            std::map<std::string, Object *> consts, vars;
            std::map<unsigned long, std::pair<Object *, Object *>> consts2, vars2;

            Object *&operator[](const std::string &name);

            bool has(const std::string &name);

            Object *&operator[](Object *);

            bool has(Object *);

            bool isConstant() const;


            virtual std::string toRawString();

            virtual String *toString(vm::Stack *s);


            virtual bool toRawBool();

            virtual Boolean *toBool(vm::Stack *s);


            void addFunction(const std::string &name, FunctionType &&function);

            void addOperator(const std::string &name, FunctionType &&function);


            void addVar(const std::string &key, Object *value);

            void addConst(const std::string &key, Object *value);


            void addVar(Object *key, Object *value);

            void addConst(Object *key, Object *value);


            void initOperators();

            virtual Object *operator_add(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_sub(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_mul(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_div(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_pow(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_rem(vm::Stack *s, Object *other) { return nullptr; }


            virtual Object *operator_and(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_or(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_xor(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_shl(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_ushl(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_shr(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_ushr(vm::Stack *s, Object *other) { return nullptr; }


            virtual Object *operator_eq(vm::Stack *s, Object *other);

            virtual Object *operator_noteq(vm::Stack *s, Object *other);

            virtual Object *operator_lt(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_le(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_gt(vm::Stack *s, Object *other) { return nullptr; }

            virtual Object *operator_ge(vm::Stack *s, Object *other) { return nullptr; }


            virtual Object *operator_inc(vm::Stack *s) { return nullptr; }

            virtual Object *operator_dec(vm::Stack *s) { return nullptr; }

            virtual Object *operator_negate(vm::Stack *s) { return nullptr; }

            virtual Object *operator_not(vm::Stack *s);

            virtual Object *operator_invert(vm::Stack *s) { return nullptr; }
        };


        class Null : public Object {
        public:
            Null();

            unsigned long hashCode() override;

            ObjectType getType() override;

            std::string toRawString() override;

            Null *toNull(vm::Stack *s) override;

            Object *operator_not(vm::Stack *s) override;

            bool toRawBool() override;

            Object *operator_eq(vm::Stack *s, Object *other) override;

            Object *operator_noteq(vm::Stack *s, Object *other) override;
        };


        class Boolean : public Object {
        public:
            static Boolean *True, *False;

            bool value;

            explicit Boolean(bool value);

            unsigned long hashCode() override;

            ObjectType getType() override;

            std::string toRawString() override;

            bool toRawBool() override;

            Object *operator_not(vm::Stack *s) override;
        };


        template<typename T>
        class Number : public Object {
        protected:
            explicit Number(T value) : Object(), value(value) {
                initOperators();
            }

        public:
            T value;

            unsigned long hashCode() override {
                return std::hash<T>()(value);
            }

            std::string toRawString() override {
                return std::to_string(value);
            }
        };

        class Char : public Number<char> {
        public:
            static Char *Zero, *One;

            explicit Char(char value) : Number(value) {}

            ObjectType getType() override;


            Char *toChar(vm::Stack *s) override;

            Int *toInt(vm::Stack *s) override;

            Float *toFloat(vm::Stack *s) override;


            Object *operator_add(vm::Stack *s, Object *other) override;

            Object *operator_sub(vm::Stack *s, Object *other) override;

            Object *operator_mul(vm::Stack *s, Object *other) override;

            Object *operator_div(vm::Stack *s, Object *other) override;

            Object *operator_pow(vm::Stack *s, Object *other) override;

            Object *operator_rem(vm::Stack *s, Object *other) override;
        };

        class Int : public Number<int> {
        public:
            static Int *One, *Zero;

            explicit Int(int v);

            ObjectType getType() override;
        };

        class Float : public Number<float> {
        public:
            static Float *Infinity, *NegativeInfinity;

            explicit Float(float v);

            ObjectType getType() override;
        };


        class Array : public Object {
            unsigned long prime;
        public:
            explicit Array(unsigned long primeNumber);

            unsigned long hashCode() override;

            ObjectType getType() override;

            void init(vm::Stack *s);

            void add(vm::Stack *s, Object *o);

            Int *length(vm::Stack *s);
        };


        class Map : public Object {
            unsigned long prime;
        public:
            explicit Map(unsigned long primeNumber);

            unsigned long hashCode() override;

            ObjectType getType() override;

            void init(vm::Stack *s);

            void put(vm::Stack *s, Object *key, Object *value);

            Int *size(vm::Stack *s);
        };


        class String : public Object {
        public:
            static String *Empty;

            std::string value;

            explicit String(const std::string &value);

            unsigned long hashCode() override;

            ObjectType getType() override;

            void init(vm::Stack *s);

            Int *length(vm::Stack *s);
        };


        class Function : public Object {
            bool builtIn;
        public:
            FunctionType function;

            explicit Function(FunctionType &&function, bool isBuiltIn = false);

            unsigned long hashCode() override;

            ObjectType getType() override;

            bool isBuiltIn() const;

            Object *call(vm::Stack *s, std::vector<Object *> &args);
        };
    }

}

#endif //E9LANG_PROJECT_OBJECTS_HPP
