#ifndef SEMANTICS_VARIABLE_HPP
#define SEMANTICS_VARIABLE_HPP

#include "ASTNode.hpp"

namespace semantics {

class Variable : public AssignableASTNode {
public:
    Variable(std::string targetVar);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;
    virtual void assign(runtime::Environment& state, std::shared_ptr<core::Object> value) override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Variable(" + targetVar_ + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~Variable() = default;

private:
    std::string targetVar_;
};
}

#endif // SEMANTICS_VARIABLE_HPP
