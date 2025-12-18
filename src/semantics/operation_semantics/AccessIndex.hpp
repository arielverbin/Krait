#ifndef SEMANTICS_ACCESS_INDEX_HPP
#define SEMANTICS_ACCESS_INDEX_HPP

#include "semantics/ASTNode.hpp"

namespace semantics {

class AccessIndex : public AssignableASTNode {
public:
    AccessIndex(std::shared_ptr<ASTNode> expression, std::shared_ptr<ASTNode> index);

    virtual core::Object* evaluate(runtime::Frame& state) const override;
    virtual void assign(runtime::Frame& state, core::Object* value) override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        return "AccessIndex(" + exp_->stringify() + ", " + index_->stringify() + ")";
    }
    #endif // KRAIT_TESTING

    virtual ~AccessIndex() = default;
    
private:
    std::shared_ptr<ASTNode> exp_;
    std::shared_ptr<ASTNode> index_;
};

} // namespace semantics

#endif // SEMANTICS_ACCESS_INDEX_HPP
