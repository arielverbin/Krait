#ifndef SEMANTICS_CALL_HPP
#define SEMANTICS_CALL_HPP

#include <vector>
#include "semantics/ASTNode.hpp"
#include "../../utils/utils.hpp"

namespace semantics {
class Call : public ASTNode {
public:
    Call(std::shared_ptr<ASTNode> callee, std::vector<std::shared_ptr<ASTNode>> args);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Call(" + callee_->stringify() + ", " +
            utils::join(utils::map<std::string, std::shared_ptr<ASTNode>>(args_, [](const auto& n) { return n->stringify(); }), ", ")
            + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~Call() = default;

private:
    std::shared_ptr<ASTNode> callee_;
    std::vector<std::shared_ptr<ASTNode>> args_;
};

}  // namespace semantics

#endif // SEMANTICS_CALL_HPP
