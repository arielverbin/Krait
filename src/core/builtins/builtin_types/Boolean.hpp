#ifndef CORE_BOOLEAN_HPP
#define CORE_BOOLEAN_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"

namespace core {

class Boolean : public utils::EnableSharedFromThis<Object, Boolean> {
public:
    Boolean(bool value);
    // Singleton access
    static std::shared_ptr<Boolean> get(bool value);
    operator bool() const;

    // Operations supported (optimization)
    std::shared_ptr<String> toString() override;
    std::shared_ptr<Boolean> toBool() override;
    std::shared_ptr<Object> logicalAnd(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> logicalOr(std::shared_ptr<Object> other) override;
    std::shared_ptr<Object> logicalNot() override;
    std::shared_ptr<Object> equal(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> notEqual(std::shared_ptr<Object> another) override;

    // Operations supported
    static std::shared_ptr<Object> toStringOp(const CallArgs& args);
    static std::shared_ptr<Object> toBoolOp(const CallArgs& args);
    static std::shared_ptr<Object> logicalAndOp(const CallArgs& args);
    static std::shared_ptr<Object> logicalOrOp(const CallArgs& args);
    static std::shared_ptr<Object> logicalNotOp(const CallArgs& args);
    static std::shared_ptr<Object> equalOp(const CallArgs& args);
    static std::shared_ptr<Object> notEqualOp(const CallArgs& args);

    static std::shared_ptr<Object> createNewOp(const CallArgs& args);

    virtual ~Boolean() = default;

private:
    bool value_;
};

} // namespace core

#endif // CORE_BOOLEAN_HPP