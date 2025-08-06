#ifndef SEMANTICS_CLASS_DEF_HPP
#define SEMANTICS_CLASS_DEF_HPP

#include "core/Object.hpp"
#include "semantics/ASTNode.hpp"
#include "utils/utils.hpp"

namespace semantics {

class ClassDef : public ASTNode {
public:
    ClassDef(std::string className, std::shared_ptr<ASTNode> body);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "Class(" + className_ + ", " + body_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~ClassDef() = default;

private:
    std::string className_;
    std::shared_ptr<ASTNode> body_;

};

} // namespace semantics

#endif // SEMANTICS_CLASS_DEF_HPP