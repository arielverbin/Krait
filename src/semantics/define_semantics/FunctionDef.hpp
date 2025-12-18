#ifndef SEMANTICS_FUNCTION_HPP
#define SEMANTICS_FUNCTION_HPP

#include "core/Object.hpp"
#include "semantics/ASTNode.hpp"
#include "utils/utils.hpp"

namespace semantics {

class FunctionDef : public BindableASTNode {
public:
    FunctionDef(std::string funcName, std::vector<std::string> params, std::shared_ptr<ASTNode> code);

    virtual core::Object* compute(runtime::Frame& state) const override;
    virtual std::string name() const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Function(" + funcName_ + ", " + utils::join(params_, ", ") + ", " + code_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~FunctionDef() = default;

private:
    std::string funcName_;
    std::vector<std::string> params_;
    std::shared_ptr<ASTNode> code_;
};

} // namespace semantics

#endif // SEMANTICS_FUNCTION_HPP
