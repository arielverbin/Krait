#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

#include <string>
#include <map>
#include <variant>
#include "utils/utils.hpp"

namespace core {

class TypeObject;
class String; class Boolean;
using MemberEntry = std::variant<std::shared_ptr<Object>, utils::LazyValue>;
using CallArgs = std::vector<std::shared_ptr<Object>>;

class Object {
protected:
    std::unordered_map<std::string, MemberEntry> members_;
    std::shared_ptr<TypeObject> type_;

    virtual std::shared_ptr<Object> getAttributeRaw(const std::string& varName);
    virtual std::shared_ptr<Object> getTypeAttribute(const std::string& varName);
    virtual bool hasAttribute(const std::string& varName);

public:
    Object(std::shared_ptr<TypeObject> type);
    virtual std::shared_ptr<Object> _shared_from_this() = 0;
    
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(Object&&) = delete;

    virtual std::shared_ptr<TypeObject> type();
    virtual std::string _type_();

    // TODO: KraitClass will implement those by checking their members for "_str_" etc.
    virtual std::shared_ptr<String> toString();
    virtual std::shared_ptr<Boolean> toBool();

    virtual std::shared_ptr<Object> add(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> subtract(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> multiply(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> divide(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> modulu(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> negate();

    virtual std::shared_ptr<Object> greaterEqual(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> greater(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> lesserEqual(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> lesser(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> equal(std::shared_ptr<Object> another);
    virtual std::shared_ptr<Object> notEqual(std::shared_ptr<Object> another);

    virtual std::shared_ptr<Object> logicalAnd(std::shared_ptr<Object> other);
    virtual std::shared_ptr<Object> logicalOr(std::shared_ptr<Object> other);
    virtual std::shared_ptr<Object> logicalNot();

    virtual std::shared_ptr<Object> call(const CallArgs& args);
    virtual std::shared_ptr<Object> get(std::shared_ptr<Object> instance);

    virtual std::shared_ptr<Object> getAttribute(const std::string& varName);
    virtual void setAttribute(const std::string& varName, MemberEntry value);

    virtual ~Object() = default;
};

} // namespace core

#endif // CORE_OBJECT_HPP
