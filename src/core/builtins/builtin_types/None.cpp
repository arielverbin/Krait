#include "None.hpp"
#include "String.hpp"
#include "Boolean.hpp"
#include <iostream>
#include "core/builtins/KraitBuiltins.hpp"
#include "exceptions/exceptions.hpp"

using namespace core;

None::None()
    : utils::EnableSharedFromThis<Object, None>(KraitBuiltins::noneType) {}

std::shared_ptr<None> None::getNone() {
    static std::shared_ptr<None> none_ = std::make_shared<None>();
    return none_;
}

std::shared_ptr<Object> None::toStringOp(const CallArgs& args) {
    if (args.size() != 1) {
        throw except::InvalidArgumentException(
            "none.__str__ requires 1 argument (received " + std::to_string(args.size()) + ")");
    }
    // Always represent None as "None"
    static std::shared_ptr<String> noneStr = std::make_shared<String>("None");
    return noneStr;
}
std::shared_ptr<String> None::toString() {
    return std::dynamic_pointer_cast<String>(None::toStringOp({ _shared_from_this() }));
}

std::shared_ptr<Object> None::toBoolOp(const CallArgs& args) {
    if (args.size() != 1) {
        throw except::InvalidArgumentException(
            "none.__bool__ requires 1 argument (received " + std::to_string(args.size()) + ")");
    }
    // None is always false
    return Boolean::get(false);
}
std::shared_ptr<Boolean> None::toBool() {
    return std::dynamic_pointer_cast<Boolean>(None::toBoolOp({ _shared_from_this() }));
}

std::shared_ptr<Object> None::equalOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "none.__eq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }

    // Only None equals None
    if (std::dynamic_pointer_cast<None>(args[1])) {
        return Boolean::get(true);
    }
    return Boolean::get(false);
}
std::shared_ptr<Object> None::equal(std::shared_ptr<Object> another) {
    return None::equalOp({ _shared_from_this(), another });
}

std::shared_ptr<Object> None::notEqualOp(const CallArgs& args) {
    if (args.size() != 2) {
        throw except::InvalidArgumentException(
            "none.__neq__ requires exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    }

    // Only None equals None
    if (std::dynamic_pointer_cast<None>(args[1])) {
        return Boolean::get(false);
    }
    return Boolean::get(true);
}
std::shared_ptr<Object> None::notEqual(std::shared_ptr<Object> another) {
    return None::notEqualOp({ _shared_from_this(), another });
}

