#ifndef SEMANTICS_CONTINUE_HPP
#define SEMANTICS_CONTINUE_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class Continue : public ASTNode {
public:
    Continue() = default;
    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;

    virtual ~Continue() = default;
};

} // namespace semantics

#endif // SEMANTICS_CONTINUE_HPP
