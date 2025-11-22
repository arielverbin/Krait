#ifndef CORE_TYPE_OBJECT_HPP
#define CORE_TYPE_OBJECT_HPP

#include <string>
#include "utils/utils.hpp"
#include "Object.hpp"
#include "core/builtins/builtin_types/Function.hpp"

namespace interpreter {
class Interpreter;
}

namespace core {

class TypeObject : public Object {
private:
    std::string name_;

public:
    TypeObject(std::string name, Function::NativeFunc creator);
    const std::string& name();

    // Operations supported (optimization)
    String* toString() override;
    Object* call(const CallArgs& args) override;

    // Operations supported
    static Object* toStringOp(const CallArgs& args);
    static Object* callOp(const CallArgs& args);
};

}

#endif // CORE_TYPE_OBJECT_HPP