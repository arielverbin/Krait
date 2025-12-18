#ifndef SEMANTICS_PASS_HPP
#define SEMANTICS_PASS_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class Pass : public ASTNode {
public:
    core::Object* evaluate(runtime::Frame& state) const override;

    #ifdef KRAIT_TESTING
    std::string stringify() const override {
        return "Pass()";
    }
    #endif // KRAIT_TESTING

    virtual ~Pass() = default;
};
} // namespace semantics

#endif // SEMANTICS_PASS_HPP
