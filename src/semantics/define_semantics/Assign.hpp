#ifndef SEMANTICS_ASSIGN_HPP
#define SEMANTICS_ASSIGN_HPP

#include "core/Object.hpp"
#include "semantics/ASTNode.hpp"

namespace semantics {

class Assign : public ASTNode {
public:
    Assign(std::shared_ptr<AssignableASTNode> target, std::shared_ptr<ASTNode> source);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~Assign() = default;

private:
    std::shared_ptr<AssignableASTNode> target_;
    std::shared_ptr<ASTNode> source_;
};

} // namespace semantics

#endif // SEMANTICS_ASSIGN_HPP
