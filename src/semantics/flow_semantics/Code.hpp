#ifndef SEMANTICS_CODE_HPP
#define SEMANTICS_CODE_HPP

#include <vector>
#include "semantics/ASTNode.hpp"

namespace semantics {
class Code : public ASTNode {
public:
    std::vector<std::shared_ptr<ASTNode>> statements;

    Code() = default;
    Code(std::vector<std::shared_ptr<ASTNode>> stmts);

    virtual core::Object* evaluate(runtime::Environment& state) const override;

    #ifdef KRAIT_TESTING
    virtual std::string stringify() const override {
        std::string result = "Code(";
        for (const auto& stmt : statements) {
            result += stmt->stringify() + ", ";
        }
        result += ")";
        return result;
    }
    #endif // KRAIT_TESTING

    virtual ~Code() = default;
};

}  // namespace semantics

#endif // SEMANTICS_CODE_HPP
