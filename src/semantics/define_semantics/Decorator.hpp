#ifndef SEMANTICS_DECORATOR_HPP
#define SEMANTICS_DECORATOR_HPP

#include "core/Object.hpp"
#include "semantics/ASTNode.hpp"
#include "utils/utils.hpp"

namespace semantics {

class Decorator : public BindableASTNode{
public:
    Decorator(std::shared_ptr<ASTNode> decorator, std::shared_ptr<BindableASTNode> decorated);

    virtual core::Object* compute(runtime::Frame& state) const override;
    virtual std::string name() const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Decorator(" + decorator_->stringify() + ", " + decorated_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~Decorator() = default;

private:
    std::shared_ptr<ASTNode> decorator_;
    std::shared_ptr<BindableASTNode> decorated_;
};

} // namespace semantics

#endif // SEMANTICS_DECORATOR_HPP