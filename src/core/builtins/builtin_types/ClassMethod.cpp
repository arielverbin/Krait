#include "ClassMethod.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/builtin_types/Method.hpp"
#include "core/TypeObject.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

ClassMethod::ClassMethod(Function* function) : Object(KraitBuiltins::classMethodType), function_(function) {
    setAttribute("__function__", function_);
}

Object* ClassMethod::getOp(const CallArgs& args) {
    if (args.size() != 3) 
        throw except::AttributeError(
            "classmethod.__get__ requires exactly 3 arguments (received " + std::to_string(args.size()) + "");
    
    auto self = dynamic_cast<ClassMethod*>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to classmethod.__get__ must be a classmethod");
    
    auto owner = dynamic_cast<core::Object*>(args[2]);
    return gc::make_tracked<Method>(owner, self->function_);
}
Object* ClassMethod::get(Object* instance, TypeObject* owner) {
    return ClassMethod::getOp( { this, instance, owner });
}

Object* ClassMethod::createNewOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
                "classmethod.__new__ requires at exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto classType = dynamic_cast<TypeObject*>(args[0]);
    if (!classType) {
        throw except::TypeError("classmethod.__new__ expects first argument to be a type (got: '"
            + args[0]->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::classMethodType) {
        throw except::TypeError("classmethod.__new__ expects first argument to be a subclass of '"
            + KraitBuiltins::classMethodType->name() + "' (got: '" + classType->name() + "')");  
    }

    auto value = dynamic_cast<Function*>(args[1]);
    if (!value) throw except::TypeError("classmethod.__new__ expects argument to be a function");  

    return gc::make_tracked<ClassMethod>(value);
}