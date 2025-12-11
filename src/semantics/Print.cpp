#include <iostream>
#include "Print.hpp"
#include "Variable.hpp"
#include "core/gc/GCPool.hpp"
#include "core/builtins/builtin_types/None.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "exceptions/exceptions.hpp"

using namespace semantics;

Print::Print(std::shared_ptr<ASTNode> expression): expression_(std::move(expression)) {}

core::Object* Print::evaluate(runtime::Frame& state) const {
    core::Object* obj = expression_->evaluate(state);
    core::String* string = obj->toString();
    
    std::cout << static_cast<std::string>(*string) << std::endl;

    return core::None::getNone();
}
