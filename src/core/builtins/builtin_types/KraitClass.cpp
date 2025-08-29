#include "KraitClass.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

KraitClass::KraitClass(TypeObject* type) : Object(type) {}

Object* KraitClass::createNewOp(const CallArgs& args) {
    if (args.size() < 1)
        throw except::InvalidArgumentException("__new__ requires at least 1 argument (received " + std::to_string(args.size()) + ")");

    auto classType = dynamic_cast<TypeObject*>(args[0]);
    if (!classType) 
        throw except::TypeError("__new__ expects its first argument to be a 'type' (got: " + args[0]->type()->name() + ")");

    auto instance = gc::make_tracked<KraitClass>(classType);
    return instance;
}