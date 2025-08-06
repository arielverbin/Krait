#include "ClassMethod.hpp"
#include "core/builtins/builtin_types/Method.hpp"
#include "core/TypeObject.hpp"
#include "core/builtins/KraitBuiltins.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

ClassMethod::ClassMethod(std::shared_ptr<Function> function) 
    : utils::EnableSharedFromThis<Object, ClassMethod>(KraitBuiltins::classMethodType), function(function) {}

std::shared_ptr<Object> ClassMethod::getOp(const CallArgs& args) {
    if (args.size() != 3) 
        throw except::AttributeError(
            "classmethod.__get__ requires exactly 3 arguments (received " + std::to_string(args.size()) + "");
    
    auto self = std::dynamic_pointer_cast<ClassMethod>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to classmethod.__get__ must be a classmethod");
    
    auto owner = std::dynamic_pointer_cast<core::Object>(args[2]);
    return std::make_shared<Method>(owner, self->function);
}
std::shared_ptr<Object> ClassMethod::get(std::shared_ptr<Object> instance, std::shared_ptr<TypeObject> owner) {
    return ClassMethod::getOp( { _shared_from_this(), instance, owner });
}

std::shared_ptr<Object> ClassMethod::createNewOp(const CallArgs& args) {
    if (args.size() != 2)
        throw except::InvalidArgumentException(
                "classmethod.__new__ requires at exactly 2 arguments (received " + std::to_string(args.size()) + ")");
    auto classType = std::dynamic_pointer_cast<TypeObject>(args[0]);
    if (!classType) {
        throw except::TypeError("classmethod.__new__ expects first argument to be a type (got: '"
            + args[0]->type()->name() + "')"); 
    }
    
    if (classType != KraitBuiltins::classMethodType) {
        throw except::TypeError("classmethod.__new__ expects first argument to be a subclass of '"
            + KraitBuiltins::classMethodType->name() + "' (got: '" + classType->name() + "')");  
    }

    auto value = std::dynamic_pointer_cast<Function>(args[1]);
    if (!value) throw except::TypeError("classmethod.__new__ expects argument to be a function");  

    return std::make_shared<ClassMethod>(value);
}