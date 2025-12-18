#ifndef CORE_STRING_HPP
#define CORE_STRING_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"
#include <string>

namespace core {

class String: public Object {
public:
    String(std::string str);
    operator std::string() const;

    // Operations supported (optimization)
    String* toString() override;
    Boolean* toBool() override;
    Object* add(Object* another) override;
    Object* reversedAdd(Object* another) override;
    Object* multiply(Object* another) override;
    Object* reversedMultiply(Object* another) override;
    Object* equal(Object* another) override;
    Object* notEqual(Object* another) override;

    // Operations supported
    static Object* toStringOp(const CallArgs& args);
    static Object* toBoolOp(const CallArgs& args);
    static Object* addOp(const CallArgs& args);
    static Object* multiplyOp(const CallArgs& args);
    static Object* equalOp(const CallArgs& args);
    static Object* notEqualOp(const CallArgs& args);
    static Object* createNewOp(const CallArgs& args);

    virtual size_t size() override { return sizeof(String); }
    virtual ~String() = default;

private:
    std::string value_;

};

} // namespace core

#endif // CORE_STRING_HPP
