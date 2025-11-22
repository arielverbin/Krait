#ifndef SEMANTICS_RETURN_HPP
#define SEMANTICS_RETURN_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class Return : public ASTNode {
public:
    Return(std::shared_ptr<ASTNode> returnValue);
    Return();

    virtual core::Object* evaluate(runtime::Frame& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Return(" + returnValue_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~Return() = default;

private:
    std::shared_ptr<ASTNode> returnValue_;
};

} // namespace semantics

#endif // SEMANTICS_RETURN_HPP
