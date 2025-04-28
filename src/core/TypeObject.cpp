#include "TypeObject.hpp"
#include "exceptions/exceptions.hpp"
using namespace core;

TypeObject::TypeObject(std::string name) :
     utils::EnableSharedFromThis<Object, TypeObject>(typeType), name_(std::move(name)) {}

std::shared_ptr<TypeObject> TypeObject::initType() {
    auto self = std::make_shared<TypeObject>("type");
    self->type_ = self;
    return self;
}

// Initialize the type 'type'.
std::shared_ptr<TypeObject> TypeObject::typeType = TypeObject::initType();

const std::string& TypeObject::name() {
    return name_;
}

std::shared_ptr<Object> TypeObject::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "type.__bool__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto self = std::dynamic_pointer_cast<TypeObject>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to type.__str__ must be a type");
    return std::make_shared<String>("<type '" + self->name() + "'>");
}
std::shared_ptr<String> TypeObject::toString() {
    return std::dynamic_pointer_cast<String>(TypeObject::toStringOp({ _shared_from_this() }));
}
