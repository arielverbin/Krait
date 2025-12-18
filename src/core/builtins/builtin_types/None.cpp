#include "None.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include <iostream>
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "exceptions/exceptions.hpp"

using namespace core;

None::None() : Object(KraitBuiltins::noneType) {}

None* None::getNone() {
    if (KraitBuiltins::noneObj == nullptr) throw except::RuntimeError("none not initialized");
    return KraitBuiltins::noneObj;
}

Object* None::toStringOp(const CallArgs& args) {
    if (args.size() != 1) {
        throw except::InvalidArgumentException(
            "none.__str__ requires 1 argument (received " + std::to_string(args.size()) + ")");
    }
    // Always represent None as "None"
    static String* noneStr = gc::make_guarded<String>("None");
    return noneStr;
}
String* None::toString() {
    return static_cast<String*>(None::toStringOp({ this }));
}

Object* None::toBoolOp(const CallArgs& args) {
    if (args.size() != 1) {
        throw except::InvalidArgumentException(
            "none.__bool__ requires 1 argument (received " + std::to_string(args.size()) + ")");
    }
    // None is always false
    return Boolean::get(false);
}
Boolean* None::toBool() {
    return static_cast<Boolean*>(None::toBoolOp({ this }));
}

Object* None::equalOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "none.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }

    // Only None equals None
    if (dynamic_cast<None*>(args[1])) {
        return Boolean::get(true);
    }
    return Boolean::get(false);
}
Object* None::equal(Object* another) {
    return None::equalOp({ this, another });
}

Object* None::notEqualOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "none.__neq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }

    // Only None equals None
    if (dynamic_cast<None*>(args[1])) {
        return Boolean::get(false);
    }
    return Boolean::get(true);
}
Object* None::notEqual(Object* another) {
    return None::notEqualOp({ this, another });
}

Object* None::createNewOp(const CallArgs& args) {
    UNREFERENCED(args);
    return None::getNone();
}
