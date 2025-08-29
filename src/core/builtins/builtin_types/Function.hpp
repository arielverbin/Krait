#ifndef CORE_FUNCTION_HPP
#define CORE_FUNCTION_HPP

#include <vector>
#include <memory>
#include <string>
#include <functional>
#include "core/Object.hpp"
#include "utils/utils.hpp"
#include "semantics/ASTNode.hpp"
#include "runtime/Environment.hpp"

namespace core {

class Function : public Object {
public:
    using NativeFunc = std::function<Object*(const CallArgs&)>;

    // Constructors for built-in functions
    Function(NativeFunc nativeFunc);

    // Constructors for user-defined functions would store an AST node, parameter list, etc.
    Function(std::shared_ptr<semantics::ASTNode> body,
             std::vector<std::string> params,
             runtime::Environment closure);

    // Operations supported (optimization)
    Object* call(const CallArgs& args) override;
    String* toString() override;
    Object* get(Object* instance, TypeObject* owner) override;

    // Operations supported
    static Object* callOp(const CallArgs& args);
    static Object* toStringOp(const CallArgs& args);
    static Object* getOp(const CallArgs& args);

    static Object* createNewOp(const CallArgs& args);

    virtual ~Function() = default;

private:
    bool isBuiltIn_;
    NativeFunc nativeFunc_;

    std::shared_ptr<semantics::ASTNode> body_;
    std::vector<std::string> params_;
    runtime::Environment closure_;
};

} // namespace core

#endif // CORE_FUNCTION_HPP

