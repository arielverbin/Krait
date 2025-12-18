#include "Code.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

Code::Code(std::vector<std::shared_ptr<ASTNode>> stmts) : statements(std::move(stmts)) {}

core::Object* Code::evaluate(runtime::Frame& state) const {
    // runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    core::Object* result = core::None::getNone();
    for (auto& stmt : statements) {
        result = stmt->evaluate(state);
    }

    return result;
}
