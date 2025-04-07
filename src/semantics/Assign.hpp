#ifndef SEMANTICS_ASSIGN_HPP
#define SEMANTICS_ASSIGN_HPP

#include "../core/Object.hpp"
#include "ASTNode.hpp"

namespace semantics {

class Assign : public ASTNode {
public:
    Assign(std::string targetVar, std::shared_ptr<ASTNode> source);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~Assign() = default;

private:
    std::string targetVar_;
    std::shared_ptr<ASTNode> source_;
};

} // namespace semantics

#endif // SEMANTICS_ASSIGN_HPP