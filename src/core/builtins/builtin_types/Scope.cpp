#include "Scope.hpp"
#include "core/TypeObject.hpp"
#include "exceptions/exceptions.hpp"
#include "core/builtins/KraitBuiltins.hpp"
using namespace core;

Scope::Scope() : Object(KraitBuiltins::scopeType, false) {
    setAttribute("__class__", type_);
}

Scope* Scope::getScope() { return this; }

Object* Scope::getMember(const std::string& varName) {
    auto member = members_.find(varName);
    if (member == members_.end()) throw except::AttributeError(
        "'" + type_->name() + "' object has no attribute '" + varName + "'");
    
    if (std::holds_alternative<LazyValue>(member->second)) {
        // It's a LazyValue — resolve and cache
        LazyValue& lazy = std::get<LazyValue>(member->second);
        Object* resolved = lazy.creator();
        member->second = resolved;
    }

    return std::get<Object*>(member->second);
}
bool Scope::hasMember(const std::string& varName) {
    return members_.find(varName) != members_.end();
}
void Scope::setMember(const std::string& varName, MemberEntry obj) {
    members_[varName] = obj;
}

Object* Scope::toStringOp(const CallArgs& args) {}
String* Scope::toString() {}

Object* Scope::toBoolOp(const CallArgs& args) {}
Boolean* Scope::toBool() {}

Object* Scope::equalOp(const CallArgs& args) {}
Object* Scope::equal(Object* another) {}

Object* Scope::notEqualOp(const CallArgs& args) {}
Object* Scope::notEqual(Object* another) {}

Object* Scope::createNewOp(const CallArgs& args) {}