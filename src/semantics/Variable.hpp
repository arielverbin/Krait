#ifndef SEMANTICS_VARIABLE_HPP
#define SEMANTICS_VARIABLE_HPP

#include "ASTNode.hpp"

namespace semantics {

class Variable : public AssignableASTNode {
public:
    Variable(std::string targetVar);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;
    virtual void assign(runtime::Environment& state, std::shared_ptr<core::Object> value) override;

    virtual ~Variable() = default;

private:
    std::string targetVar_;
};
}

#endif // SEMANTICS_VARIABLE_HPP
