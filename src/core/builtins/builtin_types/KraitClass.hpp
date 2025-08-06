#ifndef CORE_KRAIT_CLASS_HPP
#define CORE_KRAIT_CLASS_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"

namespace core {

class KraitClass : public utils::EnableSharedFromThis<Object, KraitClass> {
public:
    KraitClass(std::shared_ptr<TypeObject> type);
    static std::shared_ptr<Object> createNewOp(const CallArgs& args);
};

} // namespace core

#endif // CORE_KRAIT_CLASS_HPP