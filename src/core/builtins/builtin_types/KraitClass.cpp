#include "KraitClass.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

KraitClass::KraitClass(std::shared_ptr<TypeObject> type) : utils::EnableSharedFromThis<Object, KraitClass>(type) {}

std::shared_ptr<Object> KraitClass::createNewOp(const CallArgs& args) {
    if (args.size() < 1)
        throw except::InvalidArgumentException("__new__ requires at least 1 argument (received " + std::to_string(args.size()) + ")");

    auto classType = std::dynamic_pointer_cast<TypeObject>(args[0]);
    if (!classType) 
        throw except::TypeError("__new__ expects its first argument to be a 'type' (got: " + args[0]->type()->name() + ")");

    auto instance = std::make_shared<KraitClass>(classType);
    return instance;
}