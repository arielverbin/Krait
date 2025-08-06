#include "ClassDef.hpp"
#include "core/TypeObject.hpp"
#include "core/builtins/builtin_types/KraitClass.hpp"
#include "core/builtins/builtin_types/Function.hpp"
#include "core/builtins/builtin_types/ClassMethod.hpp"
#include "core/builtins/builtin_types/None.hpp"
using namespace semantics;

ClassDef::ClassDef(std::string className, std::shared_ptr<ASTNode> body)
    : className_(std::move(className)), body_(std::move(body)) {}

std::shared_ptr<core::Object> ClassDef::evaluate(runtime::Environment& state) const {

    auto cls = std::make_shared<core::TypeObject>(className_, core::KraitClass::createNewOp);

    // Evaluate the body of the class in a new scope
    state.pushScope(cls->getScope());
    body_->evaluate(state);
    state.popLastScope();
    
    // Store the class in the environment
    state.setVariable(className_, cls);

    // return None as class definitions do not return a value
    return core::None::getNone();
}
