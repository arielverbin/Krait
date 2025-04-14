#include "BoundMethod.hpp"
using namespace core;

BoundMethod::BoundMethod(std::shared_ptr<Object> instance, std::shared_ptr<Function> function)
    : instance_(instance), function_(function) {}

std::shared_ptr<Object> BoundMethod::_call_(std::vector<std::shared_ptr<Object>> args) {
    // Prepend the instance (self) as the first argument for the bound method
    // TODO: make it a map between name and object.
    args.insert(args.begin(), instance_);
    return function_->_call_(args);
}
