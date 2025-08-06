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

class Function : public utils::EnableSharedFromThis<Object, Function> {
public:
    using NativeFunc = std::function<std::shared_ptr<Object>(const CallArgs&)>;

    // Constructors for built-in functions
    Function(NativeFunc nativeFunc);

    // Constructors for user-defined functions would store an AST node, parameter list, etc.
    Function(std::shared_ptr<semantics::ASTNode> body,
             std::vector<std::string> params,
             runtime::Environment closure);

    // Operations supported (optimization)
    std::shared_ptr<Object> call(const CallArgs& args) override;
    std::shared_ptr<String> toString() override;
    std::shared_ptr<Object> get(std::shared_ptr<Object> instance, std::shared_ptr<TypeObject> owner) override;

    // Operations supported
    static std::shared_ptr<Object> callOp(const CallArgs& args);
    static std::shared_ptr<Object> toStringOp(const CallArgs& args);
    static std::shared_ptr<Object> getOp(const CallArgs& args);

    static std::shared_ptr<Object> createNewOp(const CallArgs& args);

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

