#ifndef SEMANTICS_VARIABLE_HPP
#define SEMANTICS_VARIABLE_HPP

#include "ASTNode.hpp"

namespace semantics {

class Variable : public AssignableASTNode {
public:
    Variable(std::string targetVar);

    virtual core::Object* evaluate(runtime::Frame& state) const override;
    virtual void assign(runtime::Frame& state, core::Object* value) override;

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
