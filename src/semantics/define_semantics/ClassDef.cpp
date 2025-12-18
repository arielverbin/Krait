#include "ClassDef.hpp"
#include "core/TypeObject.hpp"
#include "core/gc/GarbageCollector.hpp"
#include "core/builtins/builtin_types/KraitClass.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "core/builtins/builtin_types/ClassMethod.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

ClassDef::ClassDef(std::string className, std::shared_ptr<ASTNode> body)
    : className_(std::move(className)), body_(std::move(body)) {}

std::string ClassDef::name() const {
    return className_;
}

core::Object* ClassDef::compute(runtime::Frame& state) const {
    runtime::EvalContext::EvalGuard guard = runtime::EvalContext::current().Guard();

    auto cls = gc::make_tracked<core::TypeObject>(className_, core::KraitClass::createNewOp);
    guard.protect(cls);

    // Evaluate the body of the class in a new scope
    state.pushScope(cls->getScope());
    body_->evaluate(state);
    state.popLastScope();

    return cls;
}
