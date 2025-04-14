#ifndef RUNTIME_ENVIRONMENT_HPP
#define RUNTIME_ENVIRONMENT_HPP

#include "core/Object.hpp"
#include <map>
#include <stack>

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

    Environment createChildEnvironment();
};
}

#endif // RUNTIME_ENVIRONMENT_HPP
