#include "Return.hpp"
#include "Signal.hpp"
using namespace semantics;

Return::Return(std::unique_ptr<ASTNode> returnValue) : returnValue_(std::move(returnValue)) {}

std::shared_ptr<core::Object> Return::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> value = returnValue_->evaluate(state);

    throw ReturnSignal(value);
}
