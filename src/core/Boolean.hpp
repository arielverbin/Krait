#ifndef CORE_BOOLEAN_HPP
#define CORE_BOOLEAN_HPP

#include "Object.hpp"

namespace core {

class Boolean : public Object {
private:
    bool value_;

public:
    Boolean(bool value);

    operator bool() const;

    std::string _type_() override;
    std::string _str_() override;

    virtual ~Boolean() = default;
};
} // namespace core

#endif // CORE_BOOLEAN_HPP