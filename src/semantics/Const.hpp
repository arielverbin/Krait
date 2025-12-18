#ifndef SEMANTICS_CONST_HPP
#define SEMANTICS_CONST_HPP

#include <sstream>
#include "ASTNode.hpp"

namespace semantics {
class Const : public ASTNode {
public:
    Const(core::Object* obj);

    virtual core::Object* evaluate(runtime::Frame& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Const(" + static_cast<std::string>(*obj_->toString()) + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~Const() = default;
    
private:
    core::Object* obj_;
};

} // namespace semantics

#endif // SEMANTICS_CONST_HPP
