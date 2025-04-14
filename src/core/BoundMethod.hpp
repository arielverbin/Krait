#ifndef CORE_INSTANCE_METHOD_HPP
#define CORE_INSTANCE_METHOD_HPP

#include "Object.hpp"
#include "Function.hpp"

namespace core {

class BoundMethod : public Object {
public:
    BoundMethod(std::shared_ptr<Object> instance, std::shared_ptr<Function> function);
    std::shared_ptr<Object> _call_(std::vector<std::shared_ptr<Object>> args) override;

    virtual ~BoundMethod() = default;

private:
    // The instance to which the method is bound to, and the function's implementation itself.
    std::shared_ptr<Object> instance_;
    std::shared_ptr<Function> function_;
};
}

#endif // CORE_INSTANCE_METHOD_HPP