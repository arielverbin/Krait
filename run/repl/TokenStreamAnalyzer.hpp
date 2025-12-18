#ifndef TOKEN_STREAM_ANALYZER_HPP
#define TOKEN_STREAM_ANALYZER_HPP

#include <vector>
#include "lexer/Token.hpp"

namespace run {

class TokenStreamAnalyzer {
public:
  void feed(std::vector<lexer::Token> tokens) {
    decoratorDefinition_ = (tokens.size() > 0 && tokens[0].type() == lexer::TokenType::AT);

    for (auto& token : tokens) {
        switch (token.type()) {
            case lexer::TokenType::LPAREN:       ++groupingLevel_; break;
            case lexer::TokenType::LBRACKET:     ++groupingLevel_; break;
            case lexer::TokenType::RPAREN:       --groupingLevel_; break;
            case lexer::TokenType::RBRACKET:     --groupingLevel_; break;
            case lexer::TokenType::INDENT:       ++indentStack_; break;
            case lexer::TokenType::DEDENT:       --indentStack_; break;
            case lexer::TokenType::COLON:        lastWasColon_ = true; break;
            case lexer::TokenType::NEWLINE:      break;
            default:
                lastWasColon_ = false;
        }

        buffer_.push_back(token);
    }
  }

  bool statementComplete() const {
    return groupingLevel_ <= 0 && indentStack_ <= 0 && (!lastWasColon_) && (!decoratorDefinition_);
  }

  std::vector<lexer::Token> consume() {
    auto out = buffer_;
    buffer_.clear();
    resetState();
    return out;
  }

private:
  std::vector<lexer::Token> buffer_;
  int groupingLevel_ = 0;
  int indentStack_ = 0;
  bool lastWasColon_ = false;
  bool decoratorDefinition_ = false;

  void resetState() {
    groupingLevel_ = 0;
    lastWasColon_ = false;
    decoratorDefinition_ = false;
    indentStack_ = 0;
  }
};

} // namespace run

#endif // TOKEN_STREAM_ANALYZER_HPP