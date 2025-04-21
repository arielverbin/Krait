#ifndef CORE_STRING_HPP
#define CORE_STRING_HPP

#include "Object.hpp"
#include <string>

namespace core {

class String: public Object {
public:
    String(std::string str);

    std::string _type_() override;
    std::shared_ptr<Object> _str_() override;

    std::shared_ptr<Object> _add_(Object& another) override;
    std::shared_ptr<Object> _mult_(Object& another) override;
    std::shared_ptr<Object> _eq_(Object& another) override;
    std::shared_ptr<Object> _neq_(Object& another) override;
    std::shared_ptr<Object> _bool_() override;
    operator bool() const override;

    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        os << str.value_;
        return os;
    }

    virtual ~String() = default;

private:
    std::string value_;

};

} // namespace core

#endif // CORE_STRING_HPP
