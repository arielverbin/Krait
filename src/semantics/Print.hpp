#ifndef SEMANTICS_PRINT_HPP
#define SEMANTICS_PRINT_HPP

#include "ASTNode.hpp"

namespace semantics {
class Print : public ASTNode {
public:
    Print(std::shared_ptr<ASTNode> expression);

    virtual std::shared_ptr<core::Object> evaluate(runtime::GlobalState& state) override;

private:
    std::shared_ptr<ASTNode> expression_;
};

} // namespace semantics

#endif // SEMANTICS_PRINT_HPP