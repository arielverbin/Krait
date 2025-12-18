#ifndef SEMANTICS_BREAK_HPP
#define SEMANTICS_BREAK_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class Break : public ASTNode {
public:
    Break() = default;
    virtual core::Object* evaluate(runtime::Frame& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Break()";
    }
    #endif // KRAIT_TESTING

    virtual ~Break() = default;
};

} // namespace semantics

#endif // SEMANTICS_BREAK_HPP
