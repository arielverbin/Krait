#ifndef CORE_FLOAT_HPP
#define CORE_FLOAT_HPP

#include "Object.hpp"

namespace core {

class Float : public Object {
private:
    double value_;

public:
    Float(double value);

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
    std::shared_ptr<Object> _gt_(Object& another) override;
    std::shared_ptr<Object> _le_(Object& another) override;
    std::shared_ptr<Object> _lt_(Object& another) override;
    std::shared_ptr<Object> _eq_(Object& another) override;
    std::shared_ptr<Object> _neq_(Object& another) override;

    double toRawDouble();

    virtual ~Float() = default;
};
} // namespace core

#endif // CORE_FLOAT_HPP