#ifndef CORE_STRING_HPP
#define CORE_STRING_HPP

#include "Object.hpp"
#include <string>

namespace core {

class String: public Object {
private:
    std::string value_;

public:
    String(std::string str);

    std::string _type_() override;
    std::shared_ptr<Object> _add_(Object& another) override;

    std::string _str_() override;

    virtual ~String() = default;
};

} // namespace core

#endif // CORE_STRING_HPP
