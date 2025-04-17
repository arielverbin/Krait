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

    std::shared_ptr<Object> _str_() override;

    std::shared_ptr<Object> _add_(Object& another) override;
    std::shared_ptr<Object> _sub_(Object& another) override;
    std::shared_ptr<Object> _mult_(Object& another) override;
    std::shared_ptr<Object> _div_(Object& another) override;
    std::shared_ptr<Object> _mod_(Object& another) override;
    std::shared_ptr<Object> _neg_() override;

    std::shared_ptr<Object> _bool_() override;
    operator bool() const override;

    std::shared_ptr<Object> _ge_(Object& another) override;
    std::shared_ptr<Object> _le_(Object& another) override;
    std::shared_ptr<Object> _eq_(Object& another) override;

    long toRawLong();

    virtual ~Integer() = default;
};
} // namespace core

#endif // CORE_INTEGER_HPP