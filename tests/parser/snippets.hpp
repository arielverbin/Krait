#ifdef KRAIT_TESTING
#ifndef TESTS_PARSER_SNIPPETS_HPP
#define TESTS_PARSER_SNIPPETS_HPP

#include "utils.hpp"
#include "tests/interpreter/utils.hpp"

#define getResult(name) name##Result

std::string expressionTest1 = "my_var = 5 + 4 * (2 + 1)\n";
std::shared_ptr<ASTNode> expressionTest1Result = CODE(
    ASSIGNVAR("my_var", ADD(INT(5), MULT(INT(4), ADD(INT(2), INT(1)))))
);

std::string expressionTest2 = "my_var = 1 + 2 * 3 + 4 / 5 + 6\n";
std::shared_ptr<ASTNode> expressionTest2Result = CODE(
    ASSIGNVAR("my_var", ADD(ADD(ADD(INT(1), MULT(INT(2), INT(3))), DIV(INT(4), INT(5))), INT(6)))
);

std::string expressionTest3 = "x = 10 - 3 * 2\n";
std::shared_ptr<ASTNode> expressionTest3Result = CODE(
    ASSIGNVAR("x", SUB(INT(10), MULT(INT(3), INT(2))))
);

std::string expressionTest4 = "y = (1 + 2) * (3 + 4)\n";
std::shared_ptr<ASTNode> expressionTest4Result = CODE(
    ASSIGNVAR("y", MULT(ADD(INT(1), INT(2)), ADD(INT(3), INT(4))))
);

std::string expressionTest5 = "z = 2 + 3 * 4 - 5\n";
std::shared_ptr<ASTNode> expressionTest5Result = CODE(
    ASSIGNVAR("z", SUB(ADD(INT(2), MULT(INT(3), INT(4))), INT(5)))
);

std::string expressionTest6 = "a = (8 + 2) / (3 - 1)\n";
std::shared_ptr<ASTNode> expressionTest6Result = CODE(
    ASSIGNVAR("a", DIV(ADD(INT(8), INT(2)), SUB(INT(3), INT(1))))
);

std::string expressionTest7 = "b = 1 + 2 + 3 + 4\n";
std::shared_ptr<ASTNode> expressionTest7Result = CODE(
    ASSIGNVAR("b", ADD(ADD(ADD(INT(1), INT(2)), INT(3)), INT(4)))
);

std::string expressionTest8 = "x = -5 + 3\n";
std::shared_ptr<ASTNode> expressionTest8Result = CODE(
    ASSIGNVAR("x", ADD(NEG(INT(5)), INT(3)))
);

std::string expressionTest9 = "y = -(2 + 3) * 4\n";
std::shared_ptr<ASTNode> expressionTest9Result = CODE(
    ASSIGNVAR("y", MULT(NEG(ADD(INT(2), INT(3))), INT(4)))
);

std::string expressionTest10 = "z = -1 + -2 * -3\n";
std::shared_ptr<ASTNode> expressionTest10Result = CODE(
    ASSIGNVAR("z", ADD(NEG(INT(1)), MULT(NEG(INT(2)), NEG(INT(3)))))
);

std::string expressionTest11 = "a = ((1 + 2)) * (((3)))\n";
std::shared_ptr<ASTNode> expressionTest11Result = CODE(
    ASSIGNVAR("a", MULT(ADD(INT(1), INT(2)), INT(3)))
);

std::string expressionTest12 = "b = foo + bar * (baz - 2)\n";
std::shared_ptr<ASTNode> expressionTest12Result = CODE(
    ASSIGNVAR("b", ADD(VAR("foo"), MULT(VAR("bar"), SUB(VAR("baz"), INT(2)))))
);



#endif // TESTS_PARSER_SNIPPETS_HPP
#endif // KRAIT_TESTING