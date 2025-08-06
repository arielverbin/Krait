#include "KraitBuiltins.hpp"

#include "core/builtins/builtin_types/Method.hpp"
#include "core/builtins/builtin_types/ClassMethod.hpp"
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
std::shared_ptr<TypeObject> KraitBuiltins::classMethodType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::methodType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::noneType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::intType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::floatType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::boolType = nullptr;
std::shared_ptr<TypeObject> KraitBuiltins::stringType = nullptr;

void KraitBuiltins::initializeFunctionType() {
    functionType = std::make_shared<TypeObject>("function", Function::createNewOp);

    // __get__ member of Function needs to be LazyValue, since we can't initialize a Function yet, as
    // function is just now being initialized
    functionType->setAttribute("__get__", utils::LazyValue([]() -> std::shared_ptr<Function> {
        return std::make_shared<Function>(Function::getOp);
    }));
    functionType->setAttribute("__call__", std::make_shared<Function>(Function::callOp));
    functionType->setAttribute("__str__", std::make_shared<Function>(Function::toStringOp));
}

void KraitBuiltins::initializeMethodType() {
    methodType = std::make_shared<TypeObject>("method", Method::createNewOp);

    methodType->setAttribute("__call__", std::make_shared<Function>(Method::callOp));
    methodType->setAttribute("__str__", std::make_shared<Function>(Method::toStringOp));
}

void KraitBuiltins::initializedClassMethodType() {
    classMethodType = std::make_shared<TypeObject>("classmethod", nullptr);
    
    // we need to initialize __new__, which is a classmethod, after initializing 'classmethod' type.
    classMethodType->setAttribute("__new__", std::make_shared<core::ClassMethod>(
        std::make_shared<core::Function>(ClassMethod::createNewOp))
    );

    classMethodType->setAttribute("__get__", std::make_shared<Function>(ClassMethod::getOp));
    classMethodType->setAttribute("__call__", std::make_shared<ClassMethod>(std::make_shared<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeTypeType() {
    TypeObject::typeType->setAttribute("__str__", std::make_shared<Function>(TypeObject::toStringOp));
    TypeObject::typeType->setAttribute("__call__", std::make_shared<Function>(TypeObject::callOp));
}

void KraitBuiltins::initializeNoneType() {
    noneType = std::make_shared<TypeObject>("none", None::createNewOp);

    noneType->setAttribute("__str__", std::make_shared<Function>(None::toStringOp));
    noneType->setAttribute("__bool__", std::make_shared<Function>(None::toBoolOp));
    noneType->setAttribute("__eq__", std::make_shared<Function>(None::equalOp));
    noneType->setAttribute("__neq__", std::make_shared<Function>(None::notEqualOp));
}

void KraitBuiltins::initializeIntType() {
    intType = std::make_shared<TypeObject>("int", Integer::createNewOp);

    intType->setAttribute("__str__", std::make_shared<Function>(Integer::toStringOp));
    intType->setAttribute("__bool__", std::make_shared<Function>(Integer::toBoolOp));
    intType->setAttribute("__add__", std::make_shared<Function>(Integer::addOp));
    intType->setAttribute("__radd__", std::make_shared<Function>(Integer::addOp));
    intType->setAttribute("__sub__", std::make_shared<Function>(Integer::subtractOp));
    intType->setAttribute("__rsub__", std::make_shared<Function>(Integer::subtractOp));
    intType->setAttribute("__mult__", std::make_shared<Function>(Integer::multiplyOp));
    intType->setAttribute("__rmult__", std::make_shared<Function>(Integer::multiplyOp));
    intType->setAttribute("__div__", std::make_shared<Function>(Integer::divideOp));
    intType->setAttribute("__rdiv__", std::make_shared<Function>(Integer::reversedDivideOp));
    intType->setAttribute("__mod__", std::make_shared<Function>(Integer::moduluOp));
    intType->setAttribute("__rmod__", std::make_shared<Function>(Integer::reversedModuluOp));
    intType->setAttribute("__neg__", std::make_shared<Function>(Integer::negateOp));
    intType->setAttribute("__ge__", std::make_shared<Function>(Integer::greaterEqualOp));
    intType->setAttribute("__gt__", std::make_shared<Function>(Integer::greaterOp));
    intType->setAttribute("__le__", std::make_shared<Function>(Integer::lesserEqualOp));
    intType->setAttribute("__lt__", std::make_shared<Function>(Integer::lesserOp));
    intType->setAttribute("__eq__", std::make_shared<Function>(Integer::equalOp));
    intType->setAttribute("__neq__", std::make_shared<Function>(Integer::notEqualOp));

    intType->setAttribute("__call__", std::make_shared<ClassMethod>(std::make_shared<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeFloatType() {
    floatType = std::make_shared<TypeObject>("float", Float::createNewOp);

    floatType->setAttribute("__str__", std::make_shared<Function>(Float::toStringOp));
    floatType->setAttribute("__bool__", std::make_shared<Function>(Float::toBoolOp));
    floatType->setAttribute("__add__", std::make_shared<Function>(Float::addOp));
    floatType->setAttribute("__radd__", std::make_shared<Function>(Float::addOp));
    floatType->setAttribute("__sub__", std::make_shared<Function>(Float::subtractOp));
    floatType->setAttribute("__rsub__", std::make_shared<Function>(Float::addOp));
    floatType->setAttribute("__mult__", std::make_shared<Function>(Float::multiplyOp));
    floatType->setAttribute("__rmult__", std::make_shared<Function>(Float::addOp));
    floatType->setAttribute("__div__", std::make_shared<Function>(Float::divideOp));
    floatType->setAttribute("__rdiv__", std::make_shared<Function>(Float::reversedDivideOp));
    floatType->setAttribute("__mod__", std::make_shared<Function>(Float::moduluOp));
    floatType->setAttribute("__rmod__", std::make_shared<Function>(Float::reversedModuluOp));
    floatType->setAttribute("__neg__", std::make_shared<Function>(Float::negateOp));
    floatType->setAttribute("__ge__", std::make_shared<Function>(Float::greaterEqualOp));
    floatType->setAttribute("__gt__", std::make_shared<Function>(Float::greaterOp));
    floatType->setAttribute("__le__", std::make_shared<Function>(Float::lesserEqualOp));
    floatType->setAttribute("__lt__", std::make_shared<Function>(Float::lesserOp));
    floatType->setAttribute("__eq__", std::make_shared<Function>(Float::equalOp));
    floatType->setAttribute("__neq__", std::make_shared<Function>(Float::notEqualOp));

    floatType->setAttribute("__call__", std::make_shared<ClassMethod>(std::make_shared<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeBoolType() {
    boolType = std::make_shared<TypeObject>("bool", Boolean::createNewOp);

    boolType->setAttribute("__str__", std::make_shared<Function>(Boolean::toStringOp));
    boolType->setAttribute("__bool__", std::make_shared<Function>(Boolean::toBoolOp));
    boolType->setAttribute("__eq__", std::make_shared<Function>(Boolean::equalOp));
    boolType->setAttribute("__neq__", std::make_shared<Function>(Boolean::notEqualOp));

    boolType->setAttribute("__call__", std::make_shared<ClassMethod>(std::make_shared<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeStringType() {
    stringType = std::make_shared<TypeObject>("string", String::createNewOp);

    stringType->setAttribute("__str__", std::make_shared<Function>(String::toStringOp));
    stringType->setAttribute("__bool__", std::make_shared<Function>(String::toBoolOp));
    stringType->setAttribute("__add__", std::make_shared<Function>(String::addOp));
    stringType->setAttribute("__radd__", std::make_shared<Function>(String::addOp));
    stringType->setAttribute("__mult__", std::make_shared<Function>(String::multiplyOp));
    stringType->setAttribute("__rmult__", std::make_shared<Function>(String::multiplyOp));
    stringType->setAttribute("__eq__", std::make_shared<Function>(String::equalOp));
    stringType->setAttribute("__neq__", std::make_shared<Function>(String::notEqualOp));

    stringType->setAttribute("__call__", std::make_shared<ClassMethod>(std::make_shared<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeBuiltins() {
    
    // Initialize builtin types - function's type and type's type must be first and in that order
    initializeFunctionType();
    initializeTypeType();

    // rest of types,
    initializedClassMethodType();
    initializeMethodType();
    initializeNoneType();
    initializeBoolType();
    initializeIntType();
    initializeFloatType();
    initializeStringType();
}