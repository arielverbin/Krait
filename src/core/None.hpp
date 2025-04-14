#ifndef CORE_NONE_HPP
#define CORE_NONE_HPP

#include "Object.hpp"

namespace core {

class None : public Object {
private:
    None() = default;
    friend std::shared_ptr<None> std::make_shared<None>();

public:
    std::shared_ptr<Object> _str_() override;
    operator bool() const override;
    std::shared_ptr<Object> _bool_() override;

    std::string _type_() override;

    static std::shared_ptr<None> getNone();

    virtual ~None() = default;
};

} // namespace core

#endif // CORE_NONE_HPP
