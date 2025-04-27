#include "BoundMethod.hpp"
#include "exceptions/exceptions.hpp"

using namespace core;

BoundMethod::BoundMethod(std::shared_ptr<Object> instance, std::shared_ptr<Object> callable)
    : instance_(instance), callable_(callable) {}

std::shared_ptr<Object> BoundMethod::call(std::vector<std::shared_ptr<Object>> args) {
    args.insert(args.begin(), instance_);
    return callable_->call(args);
}

std::string BoundMethod::_type_() {
    return "BoundMethod";
}

std::shared_ptr<Object> BoundMethod::toString() {
    std::ostringstream oss;
    oss << "<BoundMethod for type '" + instance_->_type_() + "'>";
    return std::make_shared<String>(oss.str());
 }
