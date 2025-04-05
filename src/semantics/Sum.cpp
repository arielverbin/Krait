#include "Sum.hpp"

using namespace semantics;

Sum::Sum(std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp) : firstExp_(firstExp), secExp_(secExp) {}

std::shared_ptr<core::Object> Sum::evaluate(runtime::GlobalState& state) {
    std::shared_ptr<core::Object> firstValue = firstExp_->evaluate(state);
    std::shared_ptr<core::Object> secondValue = secExp_->evaluate(state);

    return firstValue->_add_(*secondValue);
}