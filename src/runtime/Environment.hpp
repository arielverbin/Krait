#ifndef RUNTIME_ENVIRONMENT_HPP
#define RUNTIME_ENVIRONMENT_HPP

#include "core/Object.hpp"
#include "utils/utils.hpp"
#include "core/builtins/builtin_types/String.hpp"
#include <map>
#include <stack>
#include <iostream>

namespace runtime {

class Environment {
private:
    std::deque<std::shared_ptr<utils::Scope>> scopeStack_;

public:
    Environment() = default;
    Environment(const Environment&) = default;

    std::shared_ptr<utils::Scope> pushNewScope();
    void pushScope(std::shared_ptr<utils::Scope> scope);
    std::shared_ptr<utils::Scope> popLastScope();

    std::shared_ptr<core::Object> getVariable(std::string varName);
    void setVariable(std::string varName, std::shared_ptr<core::Object> value);

    // Explicitly define a new variable in the current scope.
    void defineVariable(std::string varName, std::shared_ptr<core::Object> value);

    #ifdef KRAIT_TESTING
    // For debugging purposes, print the current environment.
    friend std::ostream& operator<<(std::ostream& os, const Environment& env) {
        size_t numScopes = env.scopeStack_.size();
        os << "-------- " << numScopes << " Scopes" << " --------" << std::endl;
    
        for (size_t i = 0; i < numScopes; ++i) {
            const auto& scope = env.scopeStack_[numScopes - i - 1];  // Top scope first (Scope 0)
            os << "Scope " << i << ": {";
    
            bool first = true;
            for (const auto& [key, value] : *scope) {
                if (!first) os << ", ";
                first = false;

                if (std::holds_alternative<utils::LazyValue>(value)) {
                    os << key << ": [LazyValue]";
                } else {
                    auto strValue = std::get<std::shared_ptr<core::Object>>(value)->toString();
                    os << key << ": \"" << static_cast<std::string>(*strValue) << "\"";
                }
            }
            os << "}" << std::endl;
        }
    
        os << "--------------------------" << std::endl;
        return os;
    }
    #endif // KRAIT_TESTING
    

    Environment createChildEnvironment();
};
}

#endif // RUNTIME_ENVIRONMENT_HPP
