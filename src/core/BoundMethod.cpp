#include "BoundMethod.hpp"
#include "exceptions/exceptions.hpp"

using namespace core;

BoundMethod::BoundMethod(std::shared_ptr<Object> instance, std::shared_ptr<Function> function)
    : instance_(instance), function_(function), numArgs_(function_->numArgs() - 1) {}

std::shared_ptr<Object> BoundMethod::_call_(std::vector<std::shared_ptr<Object>> args) {

    if (args.size() != numArgs_) {
        throw except::InvalidArgumentException("Method call with incorrect number of arguments (expected " +
            std::to_string(numArgs_) + ", got " + std::to_string(args.size()) + ").");
    }

    args.insert(args.begin(), instance_);
    return function_->_call_(args);
}

std::string BoundMethod::_type_() {
    return "BoundMethod";
}

std::shared_ptr<Object> BoundMethod::_str_() {
    std::ostringstream oss;
    oss << "<BoundMethod for type '" + instance_->_type_() + "'>";
    return std::make_shared<String>(oss.str());
 }
