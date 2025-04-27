#ifndef CORE_STRING_HPP
#define CORE_STRING_HPP

#include "Object.hpp"
#include <string>

namespace core {

class String: public Object {
public:
    String(std::string str);

    std::string _type_() override;
    std::shared_ptr<Object> toString() override;

    std::shared_ptr<Object> add(Object& another) override;
    std::shared_ptr<Object> multiply(Object& another) override;
    std::shared_ptr<Object> equal(Object& another) override;
    std::shared_ptr<Object> notEqual(Object& another) override;
    std::shared_ptr<Object> toBool() override;
    operator bool() const override;

    std::string rawString() {
        return value_;
    }

    virtual ~String() = default;

private:
    std::string value_;

};

} // namespace core

#endif // CORE_STRING_HPP
