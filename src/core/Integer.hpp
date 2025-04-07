#ifndef CORE_INTEGER_HPP
#define CORE_INTEGER_HPP

#include "Object.hpp"

namespace core {

class Integer : public Object {
private:
    long value_;

public:
    Integer(long value);

    std::string _type_() override;
    std::shared_ptr<Object> _add_(Object& another) override;
    std::shared_ptr<Object> _sub_(Object& another) override;

    std::string _str_() override;

    virtual ~Integer() = default;
};
} // namespace core

#endif // CORE_INTEGER_HPP