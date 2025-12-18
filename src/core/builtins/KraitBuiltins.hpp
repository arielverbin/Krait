#ifndef CORE_KRAIT_BUILTINTS
#define CORE_KRAIT_BUILTINTS

#include "core/Object.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "core/builtins/builtin_types/None.hpp"

namespace core {

/**
 * KraitBuiltins is responsible for initializing all of the basic types of the lanauge.
 * This includes integers, floats, strings, functions, methods, booleans, and even the type 'type'.
 * 
 * The intializing is done at a fairly early stage of the interpreter boot, and it exposes some limitations,
 * 
 * - Before the initializing, we cannot initialize a 'frame' to run the builtin intialization on or use its evaluation context, since 
 *   the type 'frame' is yet to be initialize. Therefore, we utilize the global evaluation context, which does not belong to a frame.
 *   That means that the builtin initialization can use make_guarded, and all objects it defines will live forever (because the global
 *   evaluation context does not get flushed, and it is defined as a garbage collector root).
 * 
 * - Some types might be initialized before other types, for example we must initialize the type 'type', and then initialize
 *   other types and set THEIR types to 'type'. But when we initialize the type 'type', we still cannot set its builtin functions
 *   since 'function' is not initialized... >:(
 *   It is important to mind the order of initialization specified in KraitBuiltins::initializeBuiltins() for that matter.
 */

class KraitBuiltins {
private:
    static void declateTypeType();
    static void initializeFunctionType();
    static void initializeClassMethodType();
    static void initializeMethodType();
    static void initializeScopeType();
    static void initializedFrameType();
    static void initializeTypeType();
    static void initializeNoneType();
    static void initializeIntType();
    static void initializeFloatType();
    static void initializeBoolType();
    static void initializeStringType();
    static void initializeConsts();

public:
    static void initializeBuiltins();

    static TypeObject* typeType;
    static TypeObject* functionType;
    static TypeObject* classMethodType;
    static TypeObject* methodType;
    static TypeObject* scopeType;
    static TypeObject* frameType;
    static TypeObject* noneType;
    static TypeObject* intType;
    static TypeObject* floatType;
    static TypeObject* boolType;
    static TypeObject* stringType;

    static core::Boolean* falseObj;
    static core::Boolean* trueObj;
    static core::None* noneObj;
};

} // namespace core

#endif // CORE_KRAIT_BUILTINTS