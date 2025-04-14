#include "Code.hpp"
#include "core/None.hpp"
using namespace semantics;

Code::Code(std::vector<std::shared_ptr<ASTNode>> stmts) : statements(std::move(stmts)) {}

std::shared_ptr<core::Object> Code::evaluate(runtime::Environment& state) {
    for (auto& stmt : statements) {
        stmt->evaluate(state);
    }

    return core::None::getNone();
}
