#ifndef RUNTIME_ENVIRONMENT_HPP
#define RUNTIME_ENVIRONMENT_HPP

#include "core/Object.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include "core/builtins/builtin_types/Scope.hpp"
#include "EvalContext.hpp"
#include "core/gc/GCTrackable.hpp"
#include <map>
#include <stack>
#include <iostream>

namespace runtime {

class Frame : public core::Object {
private:
    std::deque<core::Scope*> scopeStack_;
    EvalContext* evalContext_;
    
public:
    Frame();
    Frame(const Frame&);

    EvalContext* context();

    core::Scope* pushNewScope();
    void pushScope(core::Scope* scope);
    core::Scope* popLastScope();

    core::Object* getVariable(std::string varName);
    // void setVariable(std::string varName, core::Object* value);

    // Explicitly define a new variable in the current scope.
    void defineVariable(std::string varName, core::Object* value);

    std::vector<gc::GCTrackable*> referencees() override;

    virtual size_t size() override { return sizeof(Frame); }
    virtual ~Frame() = default;
};
}

#endif // RUNTIME_ENVIRONMENT_HPP
