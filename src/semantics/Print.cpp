#include "Print.hpp"
#include "Variable.hpp"
#include "../core/None.hpp"
#include <iostream>

using namespace semantics;

Print::Print(std::shared_ptr<ASTNode> expression): expression_(expression) {}


std::shared_ptr<core::Object> Print::evaluate(runtime::Environment& state) {
    std::shared_ptr<core::Object> value = expression_->evaluate(state);
    std::cout << "PRINTED " << value->_str_() << std::endl;

    return core::None::none();
}