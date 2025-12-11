#ifndef CORE_BOOLEAN_HPP
#define CORE_BOOLEAN_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"

namespace core {

class Boolean : public Object {
public:
    Boolean(bool value);
    // Singleton access
    static Boolean* get(bool value);
    operator bool() const;

    // Operations supported (optimization)
    String* toString() override;
    Boolean* toBool() override;
    Object* equal(Object* another) override;
    Object* notEqual(Object* another) override;

    // Operations supported
    static Object* toStringOp(const CallArgs& args);
    static Object* toBoolOp(const CallArgs& args);
    static Object* equalOp(const CallArgs& args);
    static Object* notEqualOp(const CallArgs& args);

    static Object* createNewOp(const CallArgs& args);

    virtual size_t size() override { return sizeof(Boolean); }
    virtual ~Boolean() = default;

private:
    bool value_;
};

} // namespace core

#endif // CORE_BOOLEAN_HPP