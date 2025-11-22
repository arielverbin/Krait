#include "Code.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

Code::Code(std::vector<std::shared_ptr<ASTNode>> stmts) : statements(std::move(stmts)) {}

core::Object* Code::evaluate(runtime::Frame& state) const {
    for (auto& stmt : statements) {
        stmt->evaluate(state);
    }

    return core::None::getNone();
}
