#include "Scope.hpp"
#include <sstream>
#include "core/TypeObject.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "core/builtins/builtin_types/Boolean.hpp"
#include "exceptions/exceptions.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/KraitBuiltins.hpp"
using namespace core;

Scope::Scope() : Object(KraitBuiltins::scopeType, false) {}

// Scope* Scope::getScope() { return this; }

Object* Scope::getMember(const std::string& varName) {
    auto member = members_.find(varName);
    if (member == members_.end()) throw except::AttributeError(
        "'" + type_->name() + "' object has no attribute '" + varName + "'");
    
    if (std::holds_alternative<LazyValue>(member->second.value)) {
        // It's a LazyValue — resolve and cache
        LazyValue& lazy = std::get<LazyValue>(member->second.value);
        Object* resolved = lazy.creator();
        member->second.value = resolved;
    }

    return std::get<Object*>(member->second.value);
}
bool Scope::hasMember(const std::string& varName) {
    return members_.find(varName) != members_.end();
}
void Scope::setMember(const std::string& varName, AttributeEntry obj) {
    members_[varName] = { obj };
}

Object* Scope::toStringOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "scope.__str__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto self = dynamic_cast<Scope*>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to scope.__str__ must be a scope");

    std::stringstream oss;
    oss << "{";
    if (!self->members_.empty()) {
        bool first = true;
        for (const auto& [key, value] : self->members_) {
            if (!first) oss << ", ";
            oss << key << ": ";
            if (std::holds_alternative<Object*>(value.value)) {
                Object* obj = std::get<Object*>(value.value);
                if (obj == self) {
                    oss << "{...}";
                } else oss << obj->toString()->operator std::string();
            } else {
                oss << "<lazy>";
            }
            first = false;
        }
    }
    oss << "}";
    return gc::make_tracked<String>(oss.str());
}
String* Scope::toString() {
    return static_cast<String*>(Scope::toStringOp({ this }));
}

Object* Scope::toBoolOp(const CallArgs& args) {
    if (args.size() != 1)
        throw except::InvalidArgumentException(
            "scope.__bool__ requires exactly 1 argument (received " + std::to_string(args.size()) + ")");

    auto self = dynamic_cast<Scope*>(args[0]);
    if (!self)
        throw except::InvalidArgumentException("first argument to scope.__bool__ must be a scope");
    
    return Boolean::get(!self->members_.empty());
}
Boolean* Scope::toBool() {
    return static_cast<Boolean*>(Scope::toBoolOp({ this }));
}

Object* Scope::equalOp(const CallArgs& args) {
    return nullptr;
}
Object* Scope::equal(Object* another) {
    return nullptr;
}

Object* Scope::notEqualOp(const CallArgs& args) {
    return nullptr;
}
Object* Scope::notEqual(Object* another) {
    return nullptr;
}

std::vector<gc::GCTrackable*> Scope::referencees() {
    // all elements in the members_ map, which are Objects (and not LazyValue)
    std::vector<gc::GCTrackable*> refs;
    for (auto& [key, value] : members_) {
        if (std::holds_alternative<Object*>(value.value)) {
            refs.push_back(std::get<Object*>(value.value));
        }
    }
    return refs;
}