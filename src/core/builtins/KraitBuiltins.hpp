#ifndef CORE_KRAIT_BUILTINTS
#define CORE_KRAIT_BUILTINTS

#include "core/Object.hpp"

namespace core {

class KraitBuiltins {
private:
    static void initializeFunctionType();
    static void initializeClassMethodType();
    static void initializeMethodType();
    static void initializeScopeType();
    static void initializeTypeType();
    static void initializeNoneType();
    static void initializeIntType();
    static void initializeFloatType();
    static void initializeBoolType();
    static void initializeStringType();

public:
    static void initializeBuiltins();

    static TypeObject* functionType;
    static TypeObject* classMethodType;
    static TypeObject* methodType;
    static TypeObject* scopeType;
    static TypeObject* noneType;
    static TypeObject* intType;
    static TypeObject* floatType;
    static TypeObject* boolType;
    static TypeObject* stringType;
};

} // namespace core

#endif // CORE_KRAIT_BUILTINTS