#ifndef SEMANTICS_CODE_HPP
#define SEMANTICS_CODE_HPP

#include <vector>
#include "ASTNode.hpp"

namespace semantics {
class Code : public ASTNode {
public:
    std::vector<std::shared_ptr<ASTNode>> statements;

    Code(std::vector<std::shared_ptr<ASTNode>> stmts);

    virtual std::shared_ptr<core::Object> evaluate(runtime::Environment& state) override;

    virtual ~Code() = default;
};

}  // namespace semantics

#endif // SEMANTICS_CODE_HPP