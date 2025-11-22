#ifndef CORE_INSTANCE_METHOD_HPP
#define CORE_INSTANCE_METHOD_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"
#include "Function.hpp"

namespace core {

class Method : public Object {
public:
    Method(Object* instance, Object* callable);

    // Operations supported (optimization)
    Object* call(const CallArgs& args) override;
    String* toString() override;

    // Operations supported
    static Object* callOp(const CallArgs& args);
    static Object* toStringOp(const CallArgs& args);

    static Object* createNewOp(const CallArgs& args);
    virtual ~Method() = default;

private:
    Object* instance_;
    Object* callable_;
};

} // namespace core

#endif // CORE_INSTANCE_METHOD_HPP