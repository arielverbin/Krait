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

TypeObject* KraitBuiltins::typeType = nullptr;
TypeObject* KraitBuiltins::functionType = nullptr;
TypeObject* KraitBuiltins::classMethodType = nullptr;
TypeObject* KraitBuiltins::methodType = nullptr;
TypeObject* KraitBuiltins::scopeType = nullptr;
TypeObject* KraitBuiltins::frameType = nullptr;
TypeObject* KraitBuiltins::noneType = nullptr;
TypeObject* KraitBuiltins::intType = nullptr;
TypeObject* KraitBuiltins::floatType = nullptr;
TypeObject* KraitBuiltins::boolType = nullptr;
TypeObject* KraitBuiltins::stringType = nullptr;

core::Boolean* KraitBuiltins::trueObj= nullptr;
core::Boolean* KraitBuiltins::falseObj= nullptr;
core::None* KraitBuiltins::noneObj= nullptr;

void KraitBuiltins::declateTypeType() {
    typeType = gc::make_guarded<TypeObject>("type", nullptr);
    
    // re-initialize type's type object
    typeType->type_ = typeType;
    typeType->setAttribute("__class__", typeType);
}

void KraitBuiltins::initializeScopeType() {
    scopeType = gc::make_guarded<TypeObject>("scope", nullptr);

    // type 'scope' and type 'type' were initialized before type 'scope'. 
    // which means their scope's type is nullptr, so we need to re-initialize it.
    KraitBuiltins::typeType->getScope()->type_ = scopeType;
    KraitBuiltins::typeType->getScope()->setAttribute("__class__", scopeType);
    scopeType->getScope()->type_ = scopeType;
    scopeType->getScope()->setAttribute("__class__", scopeType);
}

void KraitBuiltins::initializeFunctionType() {
    functionType = gc::make_guarded<TypeObject>("function", nullptr);

    // __get__ member of Function needs to be LazyValue, since we can't initialize a Function yet, as
    // function is just now being initialized
    functionType->setAttribute("__get__", core::LazyValue([]() -> Function* {
        return gc::make_guarded<Function>(Function::getOp);
    }));
    functionType->setAttribute("__call__", gc::make_guarded<Function>(Function::callOp));
    functionType->setAttribute("__str__", gc::make_guarded<Function>(Function::toStringOp));
}

void KraitBuiltins::initializeTypeType() {
    KraitBuiltins::typeType->setAttribute("__str__", gc::make_guarded<Function>(TypeObject::toStringOp));
    KraitBuiltins::typeType->setAttribute("__call__", gc::make_guarded<Function>(TypeObject::callOp));
}

void KraitBuiltins::initializeMethodType() {
    methodType = gc::make_guarded<TypeObject>("method", Method::createNewOp);

    methodType->setAttribute("__call__", gc::make_guarded<Function>(Method::callOp));
    methodType->setAttribute("__str__", gc::make_guarded<Function>(Method::toStringOp));
}

void KraitBuiltins::initializeClassMethodType() {
    classMethodType = gc::make_guarded<TypeObject>("classmethod", nullptr);
    
    // we need to initialize __new__, which is a classmethod, after initializing 'classmethod' type.
    classMethodType->setAttribute("__new__", gc::make_guarded<core::ClassMethod>(
        gc::make_guarded<core::Function>(ClassMethod::createNewOp))
    );

    classMethodType->setAttribute("__get__", gc::make_guarded<Function>(ClassMethod::getOp));
}

void KraitBuiltins::initializeNoneType() {
    noneType = gc::make_guarded<TypeObject>("none", None::createNewOp);

    noneType->setAttribute("__str__", gc::make_guarded<Function>(None::toStringOp));
    noneType->setAttribute("__bool__", gc::make_guarded<Function>(None::toBoolOp));
    noneType->setAttribute("__eq__", gc::make_guarded<Function>(None::equalOp));
    noneType->setAttribute("__neq__", gc::make_guarded<Function>(None::notEqualOp));
}

void KraitBuiltins::initializeIntType() {
    intType = gc::make_guarded<TypeObject>("int", Integer::createNewOp);

    intType->setAttribute("__str__", gc::make_guarded<Function>(Integer::toStringOp));
    intType->setAttribute("__bool__", gc::make_guarded<Function>(Integer::toBoolOp));
    intType->setAttribute("__add__", gc::make_guarded<Function>(Integer::addOp));
    intType->setAttribute("__sub__", gc::make_guarded<Function>(Integer::subtractOp));
    intType->setAttribute("__mult__", gc::make_guarded<Function>(Integer::multiplyOp));
    intType->setAttribute("__div__", gc::make_guarded<Function>(Integer::divideOp));
    intType->setAttribute("__mod__", gc::make_guarded<Function>(Integer::moduluOp));
    intType->setAttribute("__neg__", gc::make_guarded<Function>(Integer::negateOp));
    intType->setAttribute("__ge__", gc::make_guarded<Function>(Integer::greaterEqualOp));
    intType->setAttribute("__gt__", gc::make_guarded<Function>(Integer::greaterOp));
    intType->setAttribute("__le__", gc::make_guarded<Function>(Integer::lesserEqualOp));
    intType->setAttribute("__lt__", gc::make_guarded<Function>(Integer::lesserOp));
    intType->setAttribute("__eq__", gc::make_guarded<Function>(Integer::equalOp));
    intType->setAttribute("__neq__", gc::make_guarded<Function>(Integer::notEqualOp));
}

void KraitBuiltins::initializeFloatType() {
    floatType = gc::make_guarded<TypeObject>("float", Float::createNewOp);

    floatType->setAttribute("__str__", gc::make_guarded<Function>(Float::toStringOp));
    floatType->setAttribute("__bool__", gc::make_guarded<Function>(Float::toBoolOp));
    floatType->setAttribute("__add__", gc::make_guarded<Function>(Float::addOp));
    floatType->setAttribute("__sub__", gc::make_guarded<Function>(Float::subtractOp));
    floatType->setAttribute("__mult__", gc::make_guarded<Function>(Float::multiplyOp));
    floatType->setAttribute("__div__", gc::make_guarded<Function>(Float::divideOp));
    floatType->setAttribute("__mod__", gc::make_guarded<Function>(Float::moduluOp));
    floatType->setAttribute("__neg__", gc::make_guarded<Function>(Float::negateOp));
    floatType->setAttribute("__ge__", gc::make_guarded<Function>(Float::greaterEqualOp));
    floatType->setAttribute("__gt__", gc::make_guarded<Function>(Float::greaterOp));
    floatType->setAttribute("__le__", gc::make_guarded<Function>(Float::lesserEqualOp));
    floatType->setAttribute("__lt__", gc::make_guarded<Function>(Float::lesserOp));
    floatType->setAttribute("__eq__", gc::make_guarded<Function>(Float::equalOp));
    floatType->setAttribute("__neq__", gc::make_guarded<Function>(Float::notEqualOp));
}

void KraitBuiltins::initializeBoolType() {
    boolType = gc::make_guarded<TypeObject>("bool", Boolean::createNewOp);

    boolType->setAttribute("__str__", gc::make_guarded<Function>(Boolean::toStringOp));
    boolType->setAttribute("__bool__", gc::make_guarded<Function>(Boolean::toBoolOp));
    boolType->setAttribute("__eq__", gc::make_guarded<Function>(Boolean::equalOp));
    boolType->setAttribute("__neq__", gc::make_guarded<Function>(Boolean::notEqualOp));
}

void KraitBuiltins::initializeStringType() {
    stringType = gc::make_guarded<TypeObject>("string", String::createNewOp);

    stringType->setAttribute("__str__", gc::make_guarded<Function>(String::toStringOp));
    stringType->setAttribute("__bool__", gc::make_guarded<Function>(String::toBoolOp));
    stringType->setAttribute("__add__", gc::make_guarded<Function>(String::addOp));
    stringType->setAttribute("__mult__", gc::make_guarded<Function>(String::multiplyOp));
    stringType->setAttribute("__eq__", gc::make_guarded<Function>(String::equalOp));
    stringType->setAttribute("__neq__", gc::make_guarded<Function>(String::notEqualOp));
}

void KraitBuiltins::initializedFrameType() {
    frameType = gc::make_guarded<TypeObject>("frame", nullptr);
}

void KraitBuiltins::initializeConsts() {
    trueObj = gc::make_guarded<Boolean>(true);
    falseObj = gc::make_guarded<Boolean>(false);
    noneObj = gc::make_guarded<None>();

    // TODO: add integer initialization up to 516 as optimization
}

void KraitBuiltins::initializeBuiltins() {

    // Declare the fundemental object - 'type' type.
    declateTypeType();
    
    // Initialize builtin types - function's type and type's type must be first and in that order
    initializeScopeType();
    initializeFunctionType();
    initializeTypeType();

    // rest of types,
    initializeClassMethodType();
    initializeMethodType();
    initializedFrameType();
    initializeNoneType();
    initializeBoolType();
    initializeIntType();
    initializeFloatType();
    initializeStringType();

    // initialize const non-type objects (like True and False)
    initializeConsts();
}