#ifndef CORE_INSTANCE_METHOD_HPP
#define CORE_INSTANCE_METHOD_HPP

#include "Object.hpp"
#include "Function.hpp"
#include "../runtime/Environment.hpp"

namespace core {

class InstanceMethod : public Object {
public:
    InstanceMethod(std::shared_ptr<Object> instance, std::shared_ptr<Function> function)
        : instance_(instance), function_(function) {}

    std::shared_ptr<Object> _call_(runtime::Environment& state, std::vector<std::shared_ptr<Object>> args) {
        // Prepend the instance (self) as the first argument for the bound method
        args.insert(args.begin(), instance_);
        return function_->_call_(state, args);
    }

    virtual ~InstanceMethod() = default;

private:
    std::shared_ptr<Object> instance_;
    std::shared_ptr<Function> function_;
};
}

#endif // CORE_INSTANCE_METHOD_HPP