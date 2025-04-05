#ifndef SEMANTICS_CONST_HPP
#define SEMANTICS_CONST_HPP

#include "ASTNode.hpp"

namespace semantics {
class Const : public ASTNode {
public:
    Const(std::shared_ptr<core::Object> obj);

    virtual std::shared_ptr<core::Object> evaluate(runtime::GlobalState& state) override;

private:
    std::shared_ptr<core::Object> obj_;
};

} // namespace semantics

#endif // SEMANTICS_CONST_HPP