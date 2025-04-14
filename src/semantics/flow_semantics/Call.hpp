#ifndef SEMANTICS_CALL_HPP
#define SEMANTICS_CALL_HPP

#include <vector>
#include "semantics/ASTNode.hpp"

namespace semantics {
class Call : public ASTNode {
public:
    Call(std::shared_ptr<ASTNode> callee, std::vector<std::shared_ptr<ASTNode>> args);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~Call() = default;

private:
    std::shared_ptr<ASTNode> callee_;
    std::vector<std::shared_ptr<ASTNode>> args_;
};

}  // namespace semantics

#endif // SEMANTICS_CALL_HPP
