#include "UnaryOp.hpp"
#include "core/Integer.hpp"
using namespace semantics;

std::map<UnaryOpType, std::string> UnaryOp::functionTypeMap_ = {
    { UnaryOpType::Not,  "_not_"  },
    { UnaryOpType::Neg,  "_neg_"  },
    { UnaryOpType::Bool, "_bool_" },
};

UnaryOp::UnaryOp(UnaryOpType type, std::shared_ptr<ASTNode> exp)
    : type_(type), exp_(exp) {}

std::shared_ptr<core::Object> UnaryOp::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> firstValue = exp_->evaluate(state);

    // Retrieve the current object's implementation of the operation.
    std::shared_ptr<core::Object> operation = firstValue->_att_(UnaryOp::functionTypeMap_[type_]);
    return operation->_call_({});
}
