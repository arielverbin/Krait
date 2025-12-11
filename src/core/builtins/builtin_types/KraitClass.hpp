#ifndef CORE_KRAIT_CLASS_HPP
#define CORE_KRAIT_CLASS_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"

namespace core {

class KraitClass : public Object {
public:
    KraitClass(TypeObject* type);
    static Object* createNewOp(const CallArgs& args);

    virtual size_t size() override { return sizeof(KraitClass); }
    virtual ~KraitClass() = default;
};

} // namespace core

#endif // CORE_KRAIT_CLASS_HPP