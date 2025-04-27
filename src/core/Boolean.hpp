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

    bool logicalAnd(Boolean& other) const;
    bool logicalOr(Boolean& other) const;
    bool logicalNot() const;
    std::shared_ptr<Object> equal(Object& another) override;

    std::string _type_() override;
    std::shared_ptr<Object> toString() override;
    std::shared_ptr<Object> toBool() override;

    virtual ~Boolean() = default;
};
} // namespace core

#endif // CORE_BOOLEAN_HPP