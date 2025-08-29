#include "KraitBuiltins.hpp"

#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/builtin_types/Method.hpp"
#include "core/builtins/builtin_types/ClassMethod.hpp"
#include "core/builtins/builtin_types/Scope.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/Integer.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "core/builtins/builtin_types/Float.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/TypeObject.hpp"
#include "utils/utils.hpp"
using namespace core;

TypeObject* KraitBuiltins::functionType = nullptr;
TypeObject* KraitBuiltins::classMethodType = nullptr;
TypeObject* KraitBuiltins::methodType = nullptr;
TypeObject* KraitBuiltins::scopeType = nullptr;
TypeObject* KraitBuiltins::noneType = nullptr;
TypeObject* KraitBuiltins::intType = nullptr;
TypeObject* KraitBuiltins::floatType = nullptr;
TypeObject* KraitBuiltins::boolType = nullptr;
TypeObject* KraitBuiltins::stringType = nullptr;

void KraitBuiltins::initializeFunctionType() {
    functionType = gc::make_tracked<TypeObject>("function", Function::createNewOp);

    // __get__ member of Function needs to be LazyValue, since we can't initialize a Function yet, as
    // function is just now being initialized
    functionType->setAttribute("__get__", core::LazyValue([]() -> Function* {
        return gc::make_tracked<Function>(Function::getOp);
    }));
    functionType->setAttribute("__call__", gc::make_tracked<Function>(Function::callOp));
    functionType->setAttribute("__str__", gc::make_tracked<Function>(Function::toStringOp));
}

void KraitBuiltins::initializeMethodType() {
    methodType = gc::make_tracked<TypeObject>("method", Method::createNewOp);

    methodType->setAttribute("__call__", gc::make_tracked<Function>(Method::callOp));
    methodType->setAttribute("__str__", gc::make_tracked<Function>(Method::toStringOp));
}

void KraitBuiltins::initializeClassMethodType() {
    classMethodType = gc::make_tracked<TypeObject>("classmethod", nullptr);
    
    // we need to initialize __new__, which is a classmethod, after initializing 'classmethod' type.
    classMethodType->setAttribute("__new__", gc::make_tracked<core::ClassMethod>(
        gc::make_tracked<core::Function>(ClassMethod::createNewOp))
    );

    classMethodType->setAttribute("__get__", gc::make_tracked<Function>(ClassMethod::getOp));
    classMethodType->setAttribute("__call__", gc::make_tracked<ClassMethod>(gc::make_tracked<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeScopeType() {
    scopeType = gc::make_tracked<TypeObject>("scope", Scope::createNewOp);
    // type 'type' and type 'function' were initialized before type 'scope'. 
    // which means their scope's type is nullptr, so we need to re-initialize it.
    TypeObject::typeType->getScope()->type_ = scopeType;
    functionType->getScope()->type_ = scopeType;
    
    scopeType->setAttribute("__eq__", gc::make_tracked<Function>(Scope::equalOp));
    scopeType->setAttribute("__neq__", gc::make_tracked<Function>(Scope::notEqualOp));
    scopeType->setAttribute("__str__", gc::make_tracked<Function>(Scope::toStringOp));
    scopeType->setAttribute("__bool__", gc::make_tracked<Function>(Scope::toBoolOp));
}

void KraitBuiltins::initializeTypeType() {
    TypeObject::typeType->setAttribute("__str__", gc::make_tracked<Function>(TypeObject::toStringOp));
    TypeObject::typeType->setAttribute("__call__", gc::make_tracked<Function>(TypeObject::callOp));
}

void KraitBuiltins::initializeNoneType() {
    noneType = gc::make_tracked<TypeObject>("none", None::createNewOp);

    noneType->setAttribute("__str__", gc::make_tracked<Function>(None::toStringOp));
    noneType->setAttribute("__bool__", gc::make_tracked<Function>(None::toBoolOp));
    noneType->setAttribute("__eq__", gc::make_tracked<Function>(None::equalOp));
    noneType->setAttribute("__neq__", gc::make_tracked<Function>(None::notEqualOp));
}

void KraitBuiltins::initializeIntType() {
    intType = gc::make_tracked<TypeObject>("int", Integer::createNewOp);

    intType->setAttribute("__str__", gc::make_tracked<Function>(Integer::toStringOp));
    intType->setAttribute("__bool__", gc::make_tracked<Function>(Integer::toBoolOp));
    intType->setAttribute("__add__", gc::make_tracked<Function>(Integer::addOp));
    intType->setAttribute("__radd__", gc::make_tracked<Function>(Integer::addOp));
    intType->setAttribute("__sub__", gc::make_tracked<Function>(Integer::subtractOp));
    intType->setAttribute("__rsub__", gc::make_tracked<Function>(Integer::subtractOp));
    intType->setAttribute("__mult__", gc::make_tracked<Function>(Integer::multiplyOp));
    intType->setAttribute("__rmult__", gc::make_tracked<Function>(Integer::multiplyOp));
    intType->setAttribute("__div__", gc::make_tracked<Function>(Integer::divideOp));
    intType->setAttribute("__rdiv__", gc::make_tracked<Function>(Integer::reversedDivideOp));
    intType->setAttribute("__mod__", gc::make_tracked<Function>(Integer::moduluOp));
    intType->setAttribute("__rmod__", gc::make_tracked<Function>(Integer::reversedModuluOp));
    intType->setAttribute("__neg__", gc::make_tracked<Function>(Integer::negateOp));
    intType->setAttribute("__ge__", gc::make_tracked<Function>(Integer::greaterEqualOp));
    intType->setAttribute("__gt__", gc::make_tracked<Function>(Integer::greaterOp));
    intType->setAttribute("__le__", gc::make_tracked<Function>(Integer::lesserEqualOp));
    intType->setAttribute("__lt__", gc::make_tracked<Function>(Integer::lesserOp));
    intType->setAttribute("__eq__", gc::make_tracked<Function>(Integer::equalOp));
    intType->setAttribute("__neq__", gc::make_tracked<Function>(Integer::notEqualOp));

    intType->setAttribute("__call__", gc::make_tracked<ClassMethod>(gc::make_tracked<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeFloatType() {
    floatType = gc::make_tracked<TypeObject>("float", Float::createNewOp);

    floatType->setAttribute("__str__", gc::make_tracked<Function>(Float::toStringOp));
    floatType->setAttribute("__bool__", gc::make_tracked<Function>(Float::toBoolOp));
    floatType->setAttribute("__add__", gc::make_tracked<Function>(Float::addOp));
    floatType->setAttribute("__radd__", gc::make_tracked<Function>(Float::addOp));
    floatType->setAttribute("__sub__", gc::make_tracked<Function>(Float::subtractOp));
    floatType->setAttribute("__rsub__", gc::make_tracked<Function>(Float::subtractOp));
    floatType->setAttribute("__mult__", gc::make_tracked<Function>(Float::multiplyOp));
    floatType->setAttribute("__rmult__", gc::make_tracked<Function>(Float::multiplyOp));
    floatType->setAttribute("__div__", gc::make_tracked<Function>(Float::divideOp));
    floatType->setAttribute("__rdiv__", gc::make_tracked<Function>(Float::reversedDivideOp));
    floatType->setAttribute("__mod__", gc::make_tracked<Function>(Float::moduluOp));
    floatType->setAttribute("__rmod__", gc::make_tracked<Function>(Float::reversedModuluOp));
    floatType->setAttribute("__neg__", gc::make_tracked<Function>(Float::negateOp));
    floatType->setAttribute("__ge__", gc::make_tracked<Function>(Float::greaterEqualOp));
    floatType->setAttribute("__gt__", gc::make_tracked<Function>(Float::greaterOp));
    floatType->setAttribute("__le__", gc::make_tracked<Function>(Float::lesserEqualOp));
    floatType->setAttribute("__lt__", gc::make_tracked<Function>(Float::lesserOp));
    floatType->setAttribute("__eq__", gc::make_tracked<Function>(Float::equalOp));
    floatType->setAttribute("__neq__", gc::make_tracked<Function>(Float::notEqualOp));

    floatType->setAttribute("__call__", gc::make_tracked<ClassMethod>(gc::make_tracked<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeBoolType() {
    boolType = gc::make_tracked<TypeObject>("bool", Boolean::createNewOp);

    boolType->setAttribute("__str__", gc::make_tracked<Function>(Boolean::toStringOp));
    boolType->setAttribute("__bool__", gc::make_tracked<Function>(Boolean::toBoolOp));
    boolType->setAttribute("__eq__", gc::make_tracked<Function>(Boolean::equalOp));
    boolType->setAttribute("__neq__", gc::make_tracked<Function>(Boolean::notEqualOp));

    boolType->setAttribute("__call__", gc::make_tracked<ClassMethod>(gc::make_tracked<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeStringType() {
    stringType = gc::make_tracked<TypeObject>("string", String::createNewOp);

    stringType->setAttribute("__str__", gc::make_tracked<Function>(String::toStringOp));
    stringType->setAttribute("__bool__", gc::make_tracked<Function>(String::toBoolOp));
    stringType->setAttribute("__add__", gc::make_tracked<Function>(String::addOp));
    stringType->setAttribute("__radd__", gc::make_tracked<Function>(String::addOp));
    stringType->setAttribute("__mult__", gc::make_tracked<Function>(String::multiplyOp));
    stringType->setAttribute("__rmult__", gc::make_tracked<Function>(String::multiplyOp));
    stringType->setAttribute("__eq__", gc::make_tracked<Function>(String::equalOp));
    stringType->setAttribute("__neq__", gc::make_tracked<Function>(String::notEqualOp));

    stringType->setAttribute("__call__", gc::make_tracked<ClassMethod>(gc::make_tracked<Function>(TypeObject::callOp)));
}

void KraitBuiltins::initializeBuiltins() {
    
    // Initialize builtin types - function's type and type's type must be first and in that order
    initializeFunctionType();
    initializeScopeType();
    initializeTypeType();

    // rest of types,
    initializeClassMethodType();
    initializeMethodType();
    initializeNoneType();
    initializeBoolType();
    initializeIntType();
    initializeFloatType();
    initializeStringType();
}