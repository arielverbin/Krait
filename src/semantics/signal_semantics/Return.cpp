#include "Return.hpp"
#include "Signal.hpp"
#include "semantics/Const.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

Return::Return(std::shared_ptr<ASTNode> returnValue) : returnValue_(std::move(returnValue)) {}
Return::Return() : returnValue_(std::make_shared<semantics::Const>(core::None::getNone())) {}

std::shared_ptr<core::Object> Return::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> value = returnValue_->evaluate(state);

    throw ReturnSignal(value);
}
