#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

#include <string>
#include <map>
#include <variant>
#include "utils/utils.hpp"

namespace core {

class TypeObject;
class String; class Boolean;
using CallArgs = std::vector<std::shared_ptr<Object>>;

class Object {
protected:
    std::shared_ptr<TypeObject> type_;
    std::shared_ptr<utils::Scope> members_;

    std::shared_ptr<Object> getAttributeRaw(const std::string& varName);
    std::shared_ptr<Object> findAttribute(const std::string& varName);

public:
    Object(std::shared_ptr<TypeObject> type);
    virtual std::shared_ptr<Object> _shared_from_this() = 0;
    
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(Object&&) = delete;

    std::shared_ptr<TypeObject> type();
    std::shared_ptr<utils::Scope> getScope();

    std::shared_ptr<Object> getAttribute(const std::string& varName);
    void setAttribute(const std::string& varName, utils::MemberEntry value);
    bool hasAttribute(const std::string& varName);

    virtual std::shared_ptr<String> toString();
    virtual std::shared_ptr<Boolean> toBool();

    virtual std::shared_ptr<Object> add(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> reversedAdd(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> subtract(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> reversedSubtract(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> multiply(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> reversedMultiply(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> divide(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> reversedDivide(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> modulu(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> reversedModulu(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> negate();

    virtual std::shared_ptr<Object> greaterEqual(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> greater(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> lesserEqual(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> lesser(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> equal(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> notEqual(std::shared_ptr<Object> another);

    virtual std::shared_ptr<Object> call(const CallArgs& args);
    virtual std::shared_ptr<Object> get(std::shared_ptr<Object> instance, std::shared_ptr<TypeObject> owner);

    // class support
    virtual std::shared_ptr<Object> createNew(const CallArgs& args);
    virtual std::shared_ptr<Object> initialize(const CallArgs& args);

    virtual ~Object() = default;
};

} // namespace core

#endif // CORE_OBJECT_HPP
