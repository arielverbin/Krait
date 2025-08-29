#ifndef CORE_NONE_HPP
#define CORE_NONE_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"

namespace core {

class None : public Object {
public:
    None();
    static None* getNone();

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

    virtual ~None() = default;
};

} // namespace core

#endif // CORE_NONE_HPP