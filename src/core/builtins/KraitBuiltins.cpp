#include "KraitBuiltins.hpp"

#include "core/builtins/builtin_types/BoundMethod.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "core/builtins/builtin_types/Float.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/TypeObject.hpp"
#include "utils/utils.hpp"
using namespace core;

std::shared_ptr<TypeObject> KraitBuiltins::functionType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::boundMethodType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::noneType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::intType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::floatType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::boolType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::stringType = nullptr;

void KraitBuiltins::initializeFunctionType() {
    functionType = std::make_shared<TypeObject>("function");
    // __get__ member of Function needs to be LazyValue, since we can't initialize a Function yet, as
    // function is just now being initialized
    functionType->setAttribute("__get__", utils::LazyValue([]() -> std::shared_ptr<Function> {
        return std::make_shared<Function>(Function::getOp, 2);
    }));
    // number of arguments for __call__ does not matter, it is being checked by `self`.
    functionType->setAttribute("__call__", std::make_shared<Function>(Function::callOp, 0, false));
    functionType->setAttribute("__str__", std::make_shared<Function>(Function::toStringOp, 1));
}

void KraitBuiltins::initializeMethodType() {
    boundMethodType = std::make_shared<TypeObject>("method");
    // number of arguments for __call__ does not matter, it is being checked by `self`.
    boundMethodType->setAttribute("__call__", std::make_shared<Function>(BoundMethod::callOp, 0, false));
    boundMethodType->setAttribute("__str__", std::make_shared<Function>(BoundMethod::toStringOp, 1));
}

void KraitBuiltins::initializeNoneType() {
    noneType = std::make_shared<TypeObject>("none");
    noneType->setAttribute("__str__", std::make_shared<Function>(None::toStringOp, 1));
    noneType->setAttribute("__bool__", std::make_shared<Function>(None::toBoolOp, 1));
    noneType->setAttribute("__eq__", std::make_shared<Function>(None::equalOp, 2));
    noneType->setAttribute("__neq__", std::make_shared<Function>(None::notEqualOp, 2));
}

void KraitBuiltins::initializeIntType() {
    intType = std::make_shared<TypeObject>("int");
    intType->setAttribute("__str__", std::make_shared<Function>(Integer::toStringOp, 1));
    intType->setAttribute("__bool__", std::make_shared<Function>(Integer::toBoolOp, 1));
    intType->setAttribute("__add__", std::make_shared<Function>(Integer::addOp, 2));
    intType->setAttribute("__sub__", std::make_shared<Function>(Integer::subtractOp, 2));
    intType->setAttribute("__mult__", std::make_shared<Function>(Integer::multiplyOp, 2));
    intType->setAttribute("__div__", std::make_shared<Function>(Integer::divideOp, 2));
    intType->setAttribute("__mod__", std::make_shared<Function>(Integer::moduluOp, 2));
    intType->setAttribute("__neg__", std::make_shared<Function>(Integer::negateOp, 1));
    intType->setAttribute("__ge__", std::make_shared<Function>(Integer::greaterEqualOp, 2));
    intType->setAttribute("__gt__", std::make_shared<Function>(Integer::greaterOp, 2));
    intType->setAttribute("__le__", std::make_shared<Function>(Integer::lesserEqualOp, 2));
    intType->setAttribute("__lt__", std::make_shared<Function>(Integer::lesserOp, 2));
    intType->setAttribute("__eq__", std::make_shared<Function>(Integer::equalOp, 2));
    intType->setAttribute("__neq__", std::make_shared<Function>(Integer::notEqualOp, 2));
}

void KraitBuiltins::initializeFloatType() {
    floatType = std::make_shared<TypeObject>("float");
    floatType->setAttribute("__str__", std::make_shared<Function>(Float::toStringOp, 1));
    floatType->setAttribute("__bool__", std::make_shared<Function>(Float::toBoolOp, 1));
    floatType->setAttribute("__add__", std::make_shared<Function>(Float::addOp, 2));
    floatType->setAttribute("__sub__", std::make_shared<Function>(Float::subtractOp, 2));
    floatType->setAttribute("__mult__", std::make_shared<Function>(Float::multiplyOp, 2));
    floatType->setAttribute("__div__", std::make_shared<Function>(Float::divideOp, 2));
    floatType->setAttribute("__mod__", std::make_shared<Function>(Float::moduluOp, 2));
    floatType->setAttribute("__neg__", std::make_shared<Function>(Float::negateOp, 1));
    floatType->setAttribute("__ge__", std::make_shared<Function>(Float::greaterEqualOp, 2));
    floatType->setAttribute("__gt__", std::make_shared<Function>(Float::greaterOp, 2));
    floatType->setAttribute("__le__", std::make_shared<Function>(Float::lesserEqualOp, 2));
    floatType->setAttribute("__lt__", std::make_shared<Function>(Float::lesserOp, 2));
    floatType->setAttribute("__eq__", std::make_shared<Function>(Float::equalOp, 2));
    floatType->setAttribute("__neq__", std::make_shared<Function>(Float::notEqualOp, 2));
}

void KraitBuiltins::initializeBoolType() {
    boolType = std::make_shared<TypeObject>("bool");
    boolType->setAttribute("__str__", std::make_shared<Function>(Boolean::toStringOp, 1));
    boolType->setAttribute("__bool__", std::make_shared<Function>(Boolean::toBoolOp, 1));
    boolType->setAttribute("__and__", std::make_shared<Function>(Boolean::logicalAndOp, 2));
    boolType->setAttribute("__or__", std::make_shared<Function>(Boolean::logicalOrOp, 2));
    boolType->setAttribute("__not__", std::make_shared<Function>(Boolean::logicalNotOp, 1));
    boolType->setAttribute("__eq__", std::make_shared<Function>(Boolean::equalOp, 2));
    boolType->setAttribute("__neq__", std::make_shared<Function>(Boolean::notEqualOp, 2));
}

void KraitBuiltins::initializeStringType() {
    stringType = std::make_shared<TypeObject>("string");
    stringType->setAttribute("__str__", std::make_shared<Function>(String::toStringOp, 1));
    stringType->setAttribute("__bool__", std::make_shared<Function>(String::toBoolOp, 1));
    stringType->setAttribute("__add__", std::make_shared<Function>(String::addOp, 2));
    stringType->setAttribute("__mult__", std::make_shared<Function>(String::multiplyOp, 2));
    stringType->setAttribute("__eq__", std::make_shared<Function>(String::equalOp, 2));
    stringType->setAttribute("__neq__", std::make_shared<Function>(String::notEqualOp, 2));
}

void KraitBuiltins::initializeBuiltins() {

    // Initialize 'type' builtins
    TypeObject::typeType->setAttribute("__str__", std::make_shared<Function>(TypeObject::toStringOp, 1));

    // Initialize builtin types
    initializeFunctionType();
    initializeMethodType();
    initializeNoneType();
    initializeBoolType();
    initializeIntType();
    initializeFloatType();
    initializeStringType();
}