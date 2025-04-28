#ifndef CORE_INSTANCE_METHOD_HPP
#define CORE_INSTANCE_METHOD_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"
#include "Function.hpp"

namespace core {

class BoundMethod : public utils::EnableSharedFromThis<Object, BoundMethod> {
public:
    BoundMethod(std::shared_ptr<Object> instance, std::shared_ptr<Object> callable);

    std::string _type_() override;

    // Operations supported (optimization)
    std::shared_ptr<Object> call(const CallArgs& args) override;
    std::shared_ptr<String> toString() override;

    // Operations supported
    static std::shared_ptr<Object> callOp(const CallArgs& args);
    static std::shared_ptr<Object> toStringOp(const CallArgs& args);

    virtual ~BoundMethod() = default;

private:
    std::shared_ptr<Object> instance_;
    std::shared_ptr<Object> callable_;
};

} // namespace core

#endif // CORE_INSTANCE_METHOD_HPP