#ifndef RUNTIME_ENVIRONMENT_HPP
#define RUNTIME_ENVIRONMENT_HPP

#include "core/Object.hpp"
#include "core/String.hpp"
#include <map>
#include <stack>
#include <iostream>

namespace runtime {

struct Scope {
    std::map<std::string, std::shared_ptr<core::Object>> map;
};

struct CallFrame {
    struct Scope scope;
};

class Environment {
private:
    std::deque<std::shared_ptr<Scope>> scopeStack_;

public:
    Environment() = default;
    Environment(const Environment&) = default;

    std::shared_ptr<Scope> pushNewScope();
    void popLastScope();

    std::shared_ptr<core::Object> getVariable(std::string varName);
    void setVariable(std::string varName, std::shared_ptr<core::Object> value);

    // Explicitly define a new variable in the current scope.
    void defineVariable(std::string varName, std::shared_ptr<core::Object> value);

    // For debugging purposes, print the current environment.
    friend std::ostream& operator<<(std::ostream& os, const Environment& env) {
        size_t numScopes = env.scopeStack_.size();
        os << "-------- " << numScopes << " Scopes" << " --------" << std::endl;
    
        for (size_t i = 0; i < numScopes; ++i) {
            const auto& scope = env.scopeStack_[numScopes - i - 1];  // Top scope first (Scope 0)
            os << "Scope " << i << ": {";
    
            bool first = true;
            for (const auto& pair : scope->map) {
                if (!first) os << ", ";
                first = false;
    
                auto value = pair.second->toString();
    
                if (auto strValue = std::dynamic_pointer_cast<core::String>(value)) {
                    os << pair.first << ": \"" << *strValue << "\"";
                } else {
                    os << pair.first << ": " << value->_type_();
                }
            }
            os << "}" << std::endl;
        }
    
        os << "--------------------------" << std::endl;
        return os;
    }
    

    Environment createChildEnvironment();
};
}

#endif // RUNTIME_ENVIRONMENT_HPP
