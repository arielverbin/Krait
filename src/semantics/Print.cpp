#include <iostream>
#include "Print.hpp"
#include "Variable.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "exceptions/exceptions.hpp"

using namespace semantics;

Print::Print(std::shared_ptr<ASTNode> expression): expression_(std::move(expression)) {}

core::Object* Print::evaluate(runtime::Frame& state) const {
    core::String* string = expression_->evaluate(state)->toString();
    std::cout << static_cast<std::string>(*string) << std::endl;

    return core::None::getNone();
}
