#include "While.hpp"
#include "core/Boolean.hpp"
#include "core/None.hpp"
using namespace semantics;

While::While(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> block)
    : condition_(cond), block_(block) {}


std::shared_ptr<core::Object> While::evaluate(runtime::Environment& state) {
    while (*condition_->evaluate(state)->_bool_()) {
        block_->evaluate(state);
    }

    return core::None::getNone();
}
