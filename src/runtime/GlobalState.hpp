#ifndef RUNTIME_GLOBAL_STATE_HPP
#define RUNTIME_GLOBAL_STATE_HPP

#include "../core/Object.hpp"
#include <map>
#include <stack>

namespace runtime {

struct Scope {
    std::map<std::string, std::shared_ptr<core::Object>> map;
};

struct CallFrame {
    struct Scope scope;
};

class GlobalState {
private:
    std::stack<CallFrame> callStack_;
    Scope globalScope_;
public:

    Scope& currentScope();
    Scope& globalScope();

};
}

#endif // RUNTIME_GLOBAL_STATE_HPP