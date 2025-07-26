#ifndef CORE_KRAIT_BUILTINTS
#define CORE_KRAIT_BUILTINTS

#include "core/Object.hpp"

namespace core {

class KraitBuiltins {
private:
    static void initializeFunctionType();
    static void initializedClassMethodType();
    static void initializeMethodType();
    static void initializeTypeType();
    static void initializeNoneType();
    static void initializeIntType();
    static void initializeFloatType();
    static void initializeBoolType();
    static void initializeStringType();

public:
    static void initializeBuiltins();

    static std::shared_ptr<TypeObject> functionType;
    static std::shared_ptr<TypeObject> classMethodType;
    static std::shared_ptr<TypeObject> methodType;
    static std::shared_ptr<TypeObject> noneType;
    static std::shared_ptr<TypeObject> intType;
    static std::shared_ptr<TypeObject> floatType;
    static std::shared_ptr<TypeObject> boolType;
    static std::shared_ptr<TypeObject> stringType;
};

} // namespace core

#endif // CORE_KRAIT_BUILTINTS