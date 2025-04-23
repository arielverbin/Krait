#ifndef SEMANTICS_GET_PROPERTY_HPP
#define SEMANTICS_GET_PROPERTY_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class AccessProperty : public AssignableASTNode {
public:
AccessProperty(std::shared_ptr<ASTNode> expression, std::string memberName);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;
    virtual void assign(runtime::Environment& state, std::shared_ptr<core::Object> value) override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "AccessProperty(" + exp_->stringify() + ", " + memberName_ + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~AccessProperty() = default;
    
private:
    std::shared_ptr<ASTNode> exp_;
    std::string memberName_;
};

} // namespace semantics

#endif // SEMANTICS_GET_PROPERTY_HPP
