#ifndef CORE_TYPE_OBJECT_HPP
#define CORE_TYPE_OBJECT_HPP

#include <string>
#include "utils/utils.hpp"
#include "Object.hpp"

namespace core {

class TypeObject : public utils::EnableSharedFromThis<Object, TypeObject> {
private:
    std::string name_;
    static std::shared_ptr<TypeObject> initType();

public:
    TypeObject(std::string name);
    const std::string& name();
    static std::shared_ptr<TypeObject> typeType;

    // Operations supported (optimization)
    std::shared_ptr<String> toString() override;
    std::shared_ptr<Object> call(const CallArgs& args) override;

    // Operations supported
    static std::shared_ptr<Object> toStringOp(const CallArgs& args);
    static std::shared_ptr<Object> callOp(const CallArgs& args);
};

}

#endif // CORE_TYPE_OBJECT_HPP