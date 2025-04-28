#ifndef CORE_INTEGER_HPP
#define CORE_INTEGER_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"

namespace core {

class Integer : public utils::EnableSharedFromThis<Object, Integer> {
public:
    Integer(long value);
    std::string _type_() override;
    operator long() const;

    // Operations supported (optimization)
    std::shared_ptr<String> toString() override;
    std::shared_ptr<Boolean> toBool() override;
    std::shared_ptr<Object> add(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> subtract(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> multiply(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> divide(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> modulu(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> negate() override;
    std::shared_ptr<Object> greaterEqual(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> greater(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> lesserEqual(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> lesser(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> equal(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> notEqual(std::shared_ptr<Object> another) override;

    // Operations supported
    static std::shared_ptr<Object> toStringOp(const CallArgs& args);
    static std::shared_ptr<Object> toBoolOp(const CallArgs& args);
    static std::shared_ptr<Object> addOp(const CallArgs& args);
    static std::shared_ptr<Object> subtractOp(const CallArgs& args);
    static std::shared_ptr<Object> multiplyOp(const CallArgs& args);
    static std::shared_ptr<Object> divideOp(const CallArgs& args);
    static std::shared_ptr<Object> moduluOp(const CallArgs& args);
    static std::shared_ptr<Object> negateOp(const CallArgs& args);
    static std::shared_ptr<Object> greaterEqualOp(const CallArgs& args);
    static std::shared_ptr<Object> greaterOp(const CallArgs& args);
    static std::shared_ptr<Object> lesserEqualOp(const CallArgs& args);
    static std::shared_ptr<Object> lesserOp(const CallArgs& args);
    static std::shared_ptr<Object> equalOp(const CallArgs& args);
    static std::shared_ptr<Object> notEqualOp(const CallArgs& args);

private:
    long value_;
};

} // namespace core

#endif // CORE_INTEGER_HPP