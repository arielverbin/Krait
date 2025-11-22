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

core::Object* ClassDef::evaluate(runtime::Frame& state) const {

    auto cls = gc::make_tracked<core::TypeObject>(className_, core::KraitClass::createNewOp);

    // Evaluate the body of the class in a new scope
    state.pushScope(cls->getScope());
    body_->evaluate(state);
    state.popLastScope();
    
    // Store the class in the environment
    state.defineVariable(className_, cls);

    // return None as class definitions do not return a value
    return core::None::getNone();
}
