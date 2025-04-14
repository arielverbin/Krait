#ifndef CORE_FUNCTION_HPP
#define CORE_FUNCTION_HPP

#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include "Object.hpp"
#include "semantics/ASTNode.hpp"
#include "runtime/Environment.hpp"

namespace core {

using ArgList = std::vector<std::shared_ptr<Object>>;

class Function : public Object {
public:
    // For a user-defined function: store AST node pointer and parameter names
    // For a built-in, this could be a std::function<...>
    using NativeFunc = std::function<std::shared_ptr<Object>(ArgList)>;

    // Constructors for built-in functions:
    Function(NativeFunc nativeFunc);

    // Constructors for user-defined functions would store an AST node, parameter list, etc.
    Function(semantics::ASTNode* body, std::vector<std::string> params, runtime::Environment closure);

    std::shared_ptr<Object> _call_(std::vector<std::shared_ptr<Object>> args) override;

    virtual std::shared_ptr<Object> _str_() override;

    virtual ~Function() = default;
    
private:
    bool isBuiltIn_;
    NativeFunc nativeFunc_;

    // For user-defined functions:
    semantics::ASTNode* body_;
    std::vector<std::string> params_;
    runtime::Environment closure_;
};
}
#endif // CORE_FUNCTION_HPP
