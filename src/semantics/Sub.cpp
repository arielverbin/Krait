#include "Sub.hpp"

using namespace semantics;

Sub::Sub(std::shared_ptr<ASTNode> firstExp, std::shared_ptr<ASTNode> secExp) : firstExp_(firstExp), secExp_(secExp) {}

std::shared_ptr<core::Object> Sub::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> firstValue = firstExp_->evaluate(state);
    std::shared_ptr<core::Object> secondValue = secExp_->evaluate(state);

    return firstValue->_sub_(*secondValue);
}