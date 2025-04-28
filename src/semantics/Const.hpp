#ifndef SEMANTICS_CONST_HPP
#define SEMANTICS_CONST_HPP

#include <sstream>
#include "ASTNode.hpp"

namespace semantics {
class Const : public ASTNode {
public:
    Const(std::shared_ptr<core::Object> obj);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Const(" + static_cast<std::string>(*obj_->toString()) + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~Const() = default;
    
private:
    std::shared_ptr<core::Object> obj_;
};

} // namespace semantics

#endif // SEMANTICS_CONST_HPP
