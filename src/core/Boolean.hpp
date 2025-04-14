#ifndef CORE_BOOLEAN_HPP
#define CORE_BOOLEAN_HPP

#include "Object.hpp"

namespace core {

class Boolean : public Object {
private:
    bool value_;
    Boolean(bool value);
    static std::shared_ptr<Boolean> true_;
    static std::shared_ptr<Boolean> false_;
    friend std::shared_ptr<Boolean> std::make_shared<Boolean>();

public:
    static std::shared_ptr<Boolean> get(bool value);

    operator bool() const override;

    bool _and_(Boolean& other) const;
    bool _or_(Boolean& other) const;
    bool _not_() const;

    std::string _type_() override;
    std::shared_ptr<Object> _str_() override;
    std::shared_ptr<Object> _bool_() override;

    virtual ~Boolean() = default;
};
} // namespace core

#endif // CORE_BOOLEAN_HPP