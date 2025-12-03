#include "Boolean.hpp"
#include "String.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/TypeObject.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

Boolean::Boolean(bool value) : Object(KraitBuiltins::boolType), value_(value) {}

Boolean::operator bool() const {
    return value_;
}

Boolean* Boolean::get(bool value) {
    static Boolean* true_ = gc::make_tracked<Boolean>(true);
    static Boolean* false_ = gc::make_tracked<Boolean>(false);

    // TODO: move it to KraitBuiltins
    gc::GarbageCollector::instance().defineRoot(true_);
    gc::GarbageCollector::instance().defineRoot(false_);
    return value ? true_ : false_;
}

Object* Boolean::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "boolean.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = dynamic_cast<Boolean*>(args[0]);
    if (!self)
        throw except::TypeError("first argument to boolean.__str__ must be a boolean");
    
    String* trueStr = gc::make_tracked<String>("True");
    String* falseStr = gc::make_tracked<String>("False");
    return *self ? trueStr : falseStr;
}
String* Boolean::toString() {
    return static_cast<String*>(Boolean::toStringOp({ this }));
}

Object* Boolean::toBoolOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "boolean.__bool__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");
    auto self = dynamic_cast<Boolean*>(args[0]);
    if (self) return self;

    throw except::TypeError("first argument to boolean.__bool__ must be a boolean");
}
Boolean* Boolean::toBool() {
    return static_cast<Boolean*>(Boolean::toBoolOp({ this }));
}

Object* Boolean::equalOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "boolean.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Boolean*>(args[0]);
    auto b = dynamic_cast<Boolean*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "boolean.__eq__ expects both arguments to be booleans");
    return get(a->value_ == b->value_);
}
Object* Boolean::equal(Object* another) {
    return Boolean::equalOp({ this, another });
}

Object* Boolean::notEqualOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
            "boolean.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto a = dynamic_cast<Boolean*>(args[0]);
    auto b = dynamic_cast<Boolean*>(args[1]);
    if (!a || !b)
        throw except::NotImplementedException(
            "boolean.__eq__ expects both arguments to be booleans");
    return get(a->value_ != b->value_);
}
Object* Boolean::notEqual(Object* another) {
    return Boolean::notEqualOp({ this, another });
}

Object* Boolean::createNewOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
                "bool.__new__ requires at exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }
    
    auto classType = dynamic_cast<TypeObject*>(args[0]);
    if (!classType) {
        throw except::TypeError("bool.__new__ expects first argument to be a type (got: '"
            + classType->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::boolType) {
        throw except::TypeError("bool.__new__ expects first argument to be a subclass of '"
            + KraitBuiltins::boolType->name() +"' (got: '" + classType->name() + "')");  
    }

    auto value = dynamic_cast<Boolean*>(args[1]);
    if (!value) return args[1]->toBool();
    return value; 
}
