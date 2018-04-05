//
// Created by egor9814 on 30.03.18.
//

#include "objects.hpp"

e9lang::objects::Object::Object() {
    addFunction("toString", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        return toString(s);
    });
    addFunction("hashCode", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        auto hash = hashCode();
        if(hash == 0)
            return ULong::Zero;
        if(hash == 1)
            return ULong::One;
        return s->getEnv()->allocate<ULong>(hash);
    });
}

e9lang::objects::Object *&e9lang::objects::Object::operator[](const std::string &name) {
    if (vars.count(name))
        return vars[name];
    return consts[name];
}

bool e9lang::objects::Object::has(const std::string &name) {
    return vars.count(name) > 0 || consts.count(name) > 0;
}

e9lang::objects::Object *&e9lang::objects::Object::operator[](e9lang::objects::Object *o) {
    auto hash = o->hashCode();
    if (vars2.count(hash))
        return vars2[hash].second;
    return consts2[hash].second;
}

bool e9lang::objects::Object::has(e9lang::objects::Object *o) {
    auto hash = o->hashCode();
    return vars2.count(hash) > 0 || consts2.count(hash) > 0;
}

std::string e9lang::objects::Object::toRawString() {
    return "Object@" + std::to_string(hashCode());
}

e9lang::objects::String *e9lang::objects::Object::toString(vm::Stack *s) {
    auto res = toRawString();
    if(res.empty())
        return String::Empty;
    return s->getEnv()->allocate<String>(res);
}

bool e9lang::objects::Object::toRawBool() {
    return hashCode() != 0;
}

e9lang::objects::Boolean *e9lang::objects::Object::toBool(e9lang::vm::Stack *s) {
    return toRawBool() ? Boolean::True : Boolean::False;
}


void e9lang::objects::Object::addVar(const std::string &key, e9lang::objects::Object *value) {
    vars[key] = value;
    value->constant = false;
}

void e9lang::objects::Object::addConst(const std::string &key, e9lang::objects::Object *value) {
    consts[key] = value;
    value->constant = true;
}

void e9lang::objects::Object::addVar(e9lang::objects::Object *key, e9lang::objects::Object *value) {
    vars2[key->hashCode()].second = value;
    value->constant = false;
}

void e9lang::objects::Object::addConst(e9lang::objects::Object *key, e9lang::objects::Object *value) {
    consts2[key->hashCode()].second = value;
    value->constant = true;
}

bool e9lang::objects::Object::isConstant() const {
    return constant;
}

void e9lang::objects::Object::initOperators() {
    addOperator("add", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_add(s, args[0]);
    });
    addOperator("sub", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_sub(s, args[0]);
    });
    addOperator("mul", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_mul(s, args[0]);
    });
    addOperator("div", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_div(s, args[0]);
    });
    addOperator("pow", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_pow(s, args[0]);
    });
    addOperator("rem", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_rem(s, args[0]);
    });
    addOperator("and", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_and(s, args[0]);
    });
    addOperator("or", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_or(s, args[0]);
    });
    addOperator("xor", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_xor(s, args[0]);
    });
    addOperator("shl", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_shl(s, args[0]);
    });
    addOperator("ushl", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_ushl(s, args[0]);
    });
    addOperator("shr", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_shr(s, args[0]);
    });
    addOperator("ushr", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_ushr(s, args[0]);
    });
    addOperator("eq", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_eq(s, args[0]);
    });
    addOperator("noteq", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_noteq(s, args[0]);
    });
    addOperator("lt", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_lt(s, args[0]);
    });
    addOperator("le", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_le(s, args[0]);
    });
    addOperator("gt", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_gt(s, args[0]);
    });
    addOperator("ge", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        if (args.empty())
            return nullptr;
        return operator_ge(s, args[0]);
    });
    addOperator("inc", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        return operator_inc(s);
    });
    addOperator("dec", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        return operator_dec(s);
    });
    addOperator("not", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        return operator_not(s);
    });
    addOperator("invert", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        return operator_invert(s);
    });
    addOperator("negate", [&](vm::Stack *s, std::vector<Object *> &args) -> Object * {
        return operator_negate(s);
    });
}


e9lang::objects::Object *e9lang::objects::Object::operator_eq(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    return toRawBool() == other->toRawBool() ? Boolean::True : Boolean::False;
}

e9lang::objects::Object *e9lang::objects::Object::operator_noteq(e9lang::vm::Stack *s, e9lang::objects::Object *other) {
    return toRawBool() != other->toRawBool() ? Boolean::False : Boolean::True;
}

e9lang::objects::Object *e9lang::objects::Object::operator_not(e9lang::vm::Stack *s) {
    return toRawBool() ? Boolean::False : Boolean::True;
}



class BuiltInFunction : public e9lang::objects::Function {
public:
    explicit BuiltInFunction(e9lang::objects::FunctionType &&function)
            : Function(std::forward(function), true) {}
};

void e9lang::objects::Object::addFunction(const std::string &name, e9lang::objects::FunctionType &&function) {
    addVar(name, new BuiltInFunction(std::forward(function)));
}

void e9lang::objects::Object::addOperator(const std::string &name, e9lang::objects::FunctionType &&function) {
    addVar("operator_" + name, new BuiltInFunction(std::forward(function)));
}

e9lang::objects::Object::~Object() {
    for(auto &i : vars){
        if(i.second->getType() == ObjectType_FUNCTION){
            auto f = i.second->toFunction();
            if(f->isBuiltIn())
                delete f;
        }
    }
}
