#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

#include <string>
#include <map>
#include <variant>
#include "utils/utils.hpp"

namespace core {

class Object : public std::enable_shared_from_this<Object> {
protected:
    using MemberEntry = std::variant<std::shared_ptr<Object>, utils::LazyValue>;
    std::unordered_map<std::string, MemberEntry> members_;

public:
    Object();
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(Object&&) = delete;

    virtual operator bool() const;

    void* _loc_();
    virtual std::string _type_();

    // TODO: KraitClass will implement those by checking their members for "_str_" etc.
    virtual std::shared_ptr<Object> toString();
    virtual std::shared_ptr<Object> toBool();

    virtual std::shared_ptr<Object> add(Object& another);
    virtual std::shared_ptr<Object> subtract(Object& another);
    virtual std::shared_ptr<Object> multiply(Object& another);
    virtual std::shared_ptr<Object> divide(Object& another);
    virtual std::shared_ptr<Object> modulu(Object& another);
    virtual std::shared_ptr<Object> negate();

    virtual std::shared_ptr<Object> greaterEqual(Object& another);
    virtual std::shared_ptr<Object> greater(Object& another);
    virtual std::shared_ptr<Object> lesserEqual(Object& another);
    virtual std::shared_ptr<Object> lesser(Object& another);
    virtual std::shared_ptr<Object> equal(Object& another);
    virtual std::shared_ptr<Object> notEqual(Object& another);

    virtual std::shared_ptr<Object> call(std::vector<std::shared_ptr<Object>> args);

    virtual std::shared_ptr<Object> getAttribute(std::string varName);
    virtual void setAttribute(std::string varName, MemberEntry value);

    virtual ~Object() = default;
};

} // namespace core

#endif // CORE_OBJECT_HPP
