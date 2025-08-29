#ifndef SEMANTICS_PRINT_HPP
#define SEMANTICS_PRINT_HPP

#include "ASTNode.hpp"

namespace semantics {
class Print : public ASTNode {
public:
    Print(std::shared_ptr<ASTNode> expression);

    virtual core::Object* evaluate(runtime::Environment& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Print(" + expression_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~Print() = default;

private:
    std::shared_ptr<ASTNode> expression_;
};

} // namespace semantics

#endif // SEMANTICS_PRINT_HPP
