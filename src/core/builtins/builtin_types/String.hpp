#ifndef CORE_STRING_HPP
#define CORE_STRING_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"
#include <string>

namespace core {

class String: public utils::EnableSharedFromThis<Object, String> {
public:
    String(std::string str);
    operator std::string() const;

    // Operations supported (optimization)
    std::shared_ptr<String> toString() override;
    std::shared_ptr<Boolean> toBool() override;
    std::shared_ptr<Object> add(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> reversedAdd(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> multiply(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> reversedMultiply(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> equal(std::shared_ptr<Object> another) override;
    std::shared_ptr<Object> notEqual(std::shared_ptr<Object> another) override;

    // Operations supported
    static std::shared_ptr<Object> toStringOp(const CallArgs& args);
    static std::shared_ptr<Object> toBoolOp(const CallArgs& args);
    static std::shared_ptr<Object> addOp(const CallArgs& args);
    static std::shared_ptr<Object> multiplyOp(const CallArgs& args);
    static std::shared_ptr<Object> equalOp(const CallArgs& args);
    static std::shared_ptr<Object> notEqualOp(const CallArgs& args);
    static std::shared_ptr<Object> createNewOp(const CallArgs& args);

    virtual ~String() = default;

private:
    std::string value_;

};

} // namespace core

#endif // CORE_STRING_HPP
