#ifndef CORE_INSTANCE_METHOD_HPP
#define CORE_INSTANCE_METHOD_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"
#include "Function.hpp"

namespace core {

class Method : public utils::EnableSharedFromThis<Object, Method> {
public:
    Method(std::shared_ptr<Object> instance, std::shared_ptr<Object> callable);

    // Operations supported (optimization)
    std::shared_ptr<Object> call(const CallArgs& args) override;
    std::shared_ptr<String> toString() override;

    // Operations supported
    static std::shared_ptr<Object> callOp(const CallArgs& args);
    static std::shared_ptr<Object> toStringOp(const CallArgs& args);

    static std::shared_ptr<Object> createNewOp(const CallArgs& args);

    virtual ~Method() = default;

private:
    std::shared_ptr<Object> instance_;
    std::shared_ptr<Object> callable_;
};

} // namespace core

#endif // CORE_INSTANCE_METHOD_HPP