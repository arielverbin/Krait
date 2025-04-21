#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

#include <string>
#include <map>
#include <variant>

namespace core {

class Object;
struct LazyValue {
    std::function<std::shared_ptr<Object>()> creator;
    std::vector<std::shared_ptr<Object>> args;
    
    LazyValue(std::function<std::shared_ptr<Object>()> fn): creator(std::move(fn)) {}
};

class Object {
protected:
    // a workaround for being able to safely pass 'this' as a shared_ptr.
    std::shared_ptr<Object> self_;
    using MemberEntry = std::variant<std::shared_ptr<Object>, LazyValue>;

    std::map<std::string, MemberEntry> members_;

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
    virtual std::shared_ptr<Object> _str_();
    virtual std::shared_ptr<Object> _add_(Object& another);
    virtual std::shared_ptr<Object> _sub_(Object& another);
    virtual std::shared_ptr<Object> _mult_(Object& another);
    virtual std::shared_ptr<Object> _div_(Object& another);
    virtual std::shared_ptr<Object> _mod_(Object& another);
    virtual std::shared_ptr<Object> _neg_();

    virtual std::shared_ptr<Object> _bool_();
    virtual std::shared_ptr<Object> _ge_(Object& another);
    virtual std::shared_ptr<Object> _gt_(Object& another);
    virtual std::shared_ptr<Object> _le_(Object& another);
    virtual std::shared_ptr<Object> _lt_(Object& another);
    virtual std::shared_ptr<Object> _eq_(Object& another);
    virtual std::shared_ptr<Object> _neq_(Object& another);

    virtual std::shared_ptr<Object> _call_(std::vector<std::shared_ptr<Object>> args);

    virtual std::shared_ptr<Object> _att_(std::string varName);

    virtual ~Object() = default;
};

} // namespace core

#endif // CORE_OBJECT_HPP
