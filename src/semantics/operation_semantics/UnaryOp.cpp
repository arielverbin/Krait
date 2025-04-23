#include "UnaryOp.hpp"
#include "semantics/signal_semantics/Signal.hpp"
#include "core/Integer.hpp"
#include "core/None.hpp"
using namespace semantics;

std::map<UnaryOpType, std::string> UnaryOp::functionTypeMap_ = {
    { UnaryOpType::Not,  "_not_"  },
    { UnaryOpType::Neg,  "_neg_"  },
    { UnaryOpType::Bool, "_bool_" },
};

UnaryOp::UnaryOp(UnaryOpType type, std::shared_ptr<ASTNode> exp)
    : type_(type), exp_(std::move(exp)) {}

std::shared_ptr<core::Object> UnaryOp::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> firstValue = exp_->evaluate(state);

    // Retrieve the current object's implementation of the operation.
    std::shared_ptr<core::Object> operation = firstValue->_att_(UnaryOp::functionTypeMap_[type_]);
    try {
        operation->_call_({});
    } catch (const ReturnSignal& returnSignal) {
        return returnSignal.value();
    }
    return core::None::getNone();
}
