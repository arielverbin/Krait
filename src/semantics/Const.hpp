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
        if (auto strObj = std::dynamic_pointer_cast<core::String>(obj_->toString())) {
            std::ostringstream ss;
            ss << "Const(" << *strObj << ")";
            return ss.str();
        }
        return "Const(?)";
    }
    #endif // KRAIT_TESTING

    virtual ~Const() = default;
    
private:
    std::shared_ptr<core::Object> obj_;
};

} // namespace semantics

#endif // SEMANTICS_CONST_HPP
