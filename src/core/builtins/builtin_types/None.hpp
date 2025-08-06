#ifndef CORE_NONE_HPP
#define CORE_NONE_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"

namespace core {

class None : public utils::EnableSharedFromThis<Object, None> {
public:
    None();
    static std::shared_ptr<None> getNone();

    // Operations supported (optimization)
    std::shared_ptr<String> toString() override;
    std::shared_ptr<Boolean> toBool() override;
    std::shared_ptr<Object> equal(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> notEqual(std::shared_ptr<Object> another) override;

    // Operations supported
    static std::shared_ptr<Object> toStringOp(const CallArgs& args);
    static std::shared_ptr<Object> toBoolOp(const CallArgs& args);
    static std::shared_ptr<Object> equalOp(const CallArgs& args);
    static std::shared_ptr<Object> notEqualOp(const CallArgs& args);

    static std::shared_ptr<Object> createNewOp(const CallArgs& args);

    virtual ~None() = default;
};

} // namespace core

#endif // CORE_NONE_HPP