#include <iostream>
#include "Print.hpp"
#include "Variable.hpp"
#include "core/None.hpp"
#include "core/String.hpp"
#include "exceptions/exceptions.hpp"

using namespace semantics;

Print::Print(std::shared_ptr<ASTNode> expression): expression_(std::move(expression)) {}

std::shared_ptr<core::Object> Print::evaluate(runtime::Environment& state) const {
    std::shared_ptr<core::Object> string = expression_->evaluate(state)->_str_();

    if (std::shared_ptr<core::String> s = std::dynamic_pointer_cast<core::String>(string)) {
        std::cout << *s << std::endl;
        return core::None::getNone();
    }

    throw except::InvalidArgumentException("Print statement requires a String argument.");
}
