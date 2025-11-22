#ifndef CORE_CLASSMETHOD_HPP
#define CORE_CLASSMETHOD_HPP

#include "core/Object.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "utils/utils.hpp"

namespace core {

class ClassMethod : public Object {
public:
    // Constructors for built-in functions
    ClassMethod(Function* function);

    // Operations supported (optimization)
    Object* get(Object* instance, Object* owner) override;

    // Operations supported
    static Object* getOp(const CallArgs& args);
    static Object* createNewOp(const CallArgs& args);

private:
    Function* function_;
};

} // namespace core

#endif // CORE_CLASSMETHOD_HPP