#ifndef SEMANTICS_PRINT_HPP
#define SEMANTICS_PRINT_HPP

#include "ASTNode.hpp"

namespace semantics {
class Print : public ASTNode {
public:
    Print(std::unique_ptr<ASTNode> expression);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~Print() = default;

private:
    std::unique_ptr<ASTNode> expression_;
};

} // namespace semantics

#endif // SEMANTICS_PRINT_HPP
