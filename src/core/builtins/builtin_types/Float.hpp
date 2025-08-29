#ifndef CORE_FLOAT_HPP
#define CORE_FLOAT_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"

namespace core {

class Float : public Object {
public:
    Float(double value);
    operator double() const;

    // Operations supported (optimization)
    String* toString() override;
    Boolean* toBool() override;
    Object* add(Object* another) override;
    Object* reversedAdd(Object* another) override;
    Object* subtract(Object* another) override;
    Object* reversedSubtract(Object* another) override;
    Object* multiply(Object* another) override;
    Object* reversedMultiply(Object* another) override;
    Object* divide(Object* another) override;
    Object* reversedDivide(Object* another) override;
    Object* modulu(Object* another) override;
    Object* reversedModulu(Object* another) override;
    Object* negate() override;
    Object* greaterEqual(Object* another) override;
    Object* greater(Object* another) override;
    Object* lesserEqual(Object* another) override;
    Object* lesser(Object* another) override;
    Object* equal(Object* another) override;
    Object* notEqual(Object* another) override;

    // Operations supported
    static Object* toStringOp(const CallArgs& args);
    static Object* toBoolOp(const CallArgs& args);
    static Object* addOp(const CallArgs& args);
    static Object* subtractOp(const CallArgs& args);
    static Object* multiplyOp(const CallArgs& args);
    static Object* divideOp(const CallArgs& args);
    static Object* reversedDivideOp(const CallArgs& args);
    static Object* moduluOp(const CallArgs& args);
    static Object* reversedModuluOp(const CallArgs& args);
    static Object* negateOp(const CallArgs& args);
    static Object* greaterEqualOp(const CallArgs& args);
    static Object* greaterOp(const CallArgs& args);
    static Object* lesserEqualOp(const CallArgs& args);
    static Object* lesserOp(const CallArgs& args);
    static Object* equalOp(const CallArgs& args);
    static Object* notEqualOp(const CallArgs& args);

    static Object* createNewOp(const CallArgs& args);

private:
    double value_;
};

} // namespace core

#endif // CORE_FLOAT_HPP