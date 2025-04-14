#ifndef SEMANTICS_RETURN_HPP
#define SEMANTICS_RETURN_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class Return : public ASTNode {
public:
    Return(std::shared_ptr<ASTNode> returnValue);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~Return() = default;

private:
    std::shared_ptr<ASTNode> returnValue_;
};

} // namespace semantics

#endif // SEMANTICS_RETURN_HPP
