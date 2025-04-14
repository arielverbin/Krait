#ifndef SEMANTICS_FUNCTION_HPP
#define SEMANTICS_FUNCTION_HPP

#include "core/Object.hpp"
#include "semantics/ASTNode.hpp"

namespace semantics {

class Function : public ASTNode {
public:
    Function(std::string funcName, std::vector<std::string> params, std::shared_ptr<ASTNode> code);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~Function() = default;

private:
    std::string funcName_;
    std::vector<std::string> params_;
    std::shared_ptr<ASTNode> code_;
};

} // namespace semantics

#endif // SEMANTICS_FUNCTION_HPP
