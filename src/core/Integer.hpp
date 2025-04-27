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

    std::shared_ptr<Object> toString() override;

    std::shared_ptr<Object> add(Object& another) override;
    std::shared_ptr<Object> subtract(Object& another) override;
    std::shared_ptr<Object> multiply(Object& another) override;
    std::shared_ptr<Object> divide(Object& another) override;
    std::shared_ptr<Object> modulu(Object& another) override;
    std::shared_ptr<Object> negate() override;

    std::shared_ptr<Object> toBool() override;
    operator bool() const override;

    std::shared_ptr<Object> greaterEqual(Object& another) override;
    std::shared_ptr<Object> greater(Object& another) override;
    std::shared_ptr<Object> lesserEqual(Object& another) override;
    std::shared_ptr<Object> lesser(Object& another) override;
    std::shared_ptr<Object> equal(Object& another) override;
    std::shared_ptr<Object> notEqual(Object& another) override;

    long toRawLong();

    virtual ~Integer() = default;
};
} // namespace core

#endif // CORE_INTEGER_HPP