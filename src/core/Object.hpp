#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

#include <string>
#include <map>
#include <functional>
#include <memory>
#include <variant>
#include "core/gc/GCTrackable.hpp"
#include "utils/utils.hpp"

namespace core {

class KraitBuiltins;
class Object;
struct LazyValue {
    std::function<core::Object*()> creator;
    std::vector<core::Object*> args;

    LazyValue(std::function<core::Object*()> fn): creator(std::move(fn)) {}
};


class TypeObject; class Scope;
class String; class Boolean;

using CallArgs = std::vector<Object*>;
using AttributeEntry = std::variant<core::Object*, core::LazyValue>;

class Object : public gc::GCTrackable {
protected:
    TypeObject* type_;
    Scope* members_;

    Object* getAttributeRaw(const std::string& varName, Object* instance = nullptr);
    Object* getTypeAttribute(const std::string& varName);
    Object* findAttribute(const std::string& varName);
    friend class KraitBuiltins;
    friend class Interpreter;

public:
    Object(TypeObject *type);
    
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(Object&&) = delete;

    TypeObject* type();
    virtual Scope* getScope(); // for Scope class, this function returns 'this'

    virtual Object* getAttribute(const std::string& varName);
    virtual void setAttribute(const std::string& varName, AttributeEntry value);
    virtual bool hasAttribute(const std::string& varName);

    virtual String* toString();
    virtual Boolean* toBool();

    virtual Object* add(Object* another);
    virtual Object* reversedAdd(Object* another);
    virtual Object* subtract(Object* another);
    virtual Object* reversedSubtract(Object* another);
    virtual Object* multiply(Object* another);
    virtual Object* reversedMultiply(Object* another);
    virtual Object* divide(Object* another);
    virtual Object* reversedDivide(Object* another);
    virtual Object* modulu(Object* another);
    virtual Object* reversedModulu(Object* another);
    virtual Object* negate();

    virtual Object* greaterEqual(Object* another);
    virtual Object* greater(Object* another);
    virtual Object* lesserEqual(Object* another);
    virtual Object* lesser(Object* another);
    virtual Object* equal(Object* another);
    virtual Object* notEqual(Object* another);

    virtual Object* call(const CallArgs& args);
    virtual Object* get(Object* instance, Object* owner);

    // class support
    virtual Object* createNew(const CallArgs& args);
    virtual Object* initialize(const CallArgs& args);

    // garbage collector support
    std::vector<gc::GCTrackable*> referencees() override;

    virtual ~Object() = default;
};

} // namespace core

#endif // CORE_OBJECT_HPP
