#ifndef SEMANTICS_PASS_HPP
#define SEMANTICS_PASS_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class Pass : public ASTNode {
public:
    std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~Pass() = default;
};
} // namespace semantics

#endif // SEMANTICS_PASS_HPP
