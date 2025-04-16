#ifndef SEMANTICS_ASSIGN_HPP
#define SEMANTICS_ASSIGN_HPP

#include "core/Object.hpp"
#include "semantics/ASTNode.hpp"

namespace semantics {

class Assign : public ASTNode {
public:
    Assign(std::shared_ptr<AssignableASTNode> target, std::shared_ptr<ASTNode> source);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Assign(" + target_->stringify() + ", " + source_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~Assign() = default;

#ifndef KRAIT_TESTING
private:
#endif // KRAIT_TESTING
    std::shared_ptr<AssignableASTNode> target_;
    std::shared_ptr<ASTNode> source_;
};

} // namespace semantics

#endif // SEMANTICS_ASSIGN_HPP
