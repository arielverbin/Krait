#ifndef CORE_CLASSMETHOD_HPP
#define CORE_CLASSMETHOD_HPP

#include "core/Object.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "utils/utils.hpp"

namespace core {

class ClassMethod : public utils::EnableSharedFromThis<Object, ClassMethod> {
public:
    // Constructors for built-in functions
    ClassMethod(std::shared_ptr<Function> function);

    // Operations supported (optimization)
    std::shared_ptr<Object> get(std::shared_ptr<Object> instance, std::shared_ptr<TypeObject> owner) override;

    // Operations supported
    static std::shared_ptr<Object> getOp(const CallArgs& args);
    static std::shared_ptr<Object> createNewOp(const CallArgs& args);

private:
    std::shared_ptr<Function> function;
};

} // namespace core

#endif // CORE_CLASSMETHOD_HPP