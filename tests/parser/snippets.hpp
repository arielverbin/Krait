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

// 11) Handles nested parentheses
std::string expressionTest11 = "a = ((1 + 2)) * (((3)))\n";
std::shared_ptr<ASTNode> expressionTest11Result = CODE(
    ASSIGNVAR("a", MULT(ADD(INT(1), INT(2)), INT(3)))
);

// 12) Handles variables
std::string expressionTest12 = "b = foo + bar * (baz - 2)\n";
std::shared_ptr<ASTNode> expressionTest12Result = CODE(
    ASSIGNVAR("b", ADD(VAR("foo"), MULT(VAR("bar"), SUB(VAR("baz"), INT(2)))))
);

// 13) Left‑associative chain of subtraction
std::string expressionTest13 = "g = 10 - 3 - 2\n";
std::shared_ptr<ASTNode> expressionTest13Result = CODE(
    ASSIGNVAR("g",
      SUB(
        SUB(INT(10), INT(3)),
        INT(2)
      )
    )
);

// 14) Mixed multiply/divide (same precedence, left‑assoc)
std::string expressionTest14 = "h = 8 / 4 * 2\n";
std::shared_ptr<ASTNode> expressionTest14Result = CODE(
    ASSIGNVAR("h",
      MULT(
        DIV(INT(8), INT(4)),
        INT(2)
      )
    )
);

// 15) Comparison vs. arithmetic
std::string expressionTest15 = "i = a <= b + c\n";
std::shared_ptr<ASTNode> expressionTest15Result = CODE(
    ASSIGNVAR("i",
      LEQ(
        VAR("a"),
        ADD(VAR("b"), VAR("c"))
      )
    )
);

// 16) Equality chain (left‑assoc)
std::string expressionTest16 = "j = x == y == z\n";
std::shared_ptr<ASTNode> expressionTest16Result = CODE(
    ASSIGNVAR("j",
      EQ(
        EQ(VAR("x"), VAR("y")),
        VAR("z")
      )
    )
);

// 17) Logical OR/AND precedence
std::string expressionTest17 = "k = p or q and r\n";
std::shared_ptr<ASTNode> expressionTest17Result = CODE(
    ASSIGNVAR("k",
      OR(
        VAR("p"),
        AND(VAR("q"), VAR("r"))
      )
    )
);

// 18) Chained OR (left‑assoc)
std::string expressionTest18 = "l = u or v or w\n";
std::shared_ptr<ASTNode> expressionTest18Result = CODE(
    ASSIGNVAR("l",
      OR(
        OR(VAR("u"), VAR("v")),
        VAR("w")
      )
    )
);

// 19) NOT loose‑binding over assignment
std::string expressionTest19 = "m = not x = y\n";
std::shared_ptr<ASTNode> expressionTest19Result = CODE(
    ASSIGNVAR("m",
      NOT(
        ASSIGNVAR("x", VAR("y"))
      )
    )
);

// 20) NOT over comparison and arithmetic
std::string expressionTest20 = "n = not a * b == c - d\n";
std::shared_ptr<ASTNode> expressionTest20Result = CODE(
    ASSIGNVAR("n",
      NOT(
        EQ(
          MULT(VAR("a"), VAR("b")),
          SUB(VAR("c"), VAR("d"))
        )
      )
    )
);

// 21) Assignment is left-associative
std::string expressionTest21 = "a = b = c\n";
std::shared_ptr<ASTNode> expressionTest21Result = CODE(
    ASSIGNVAR("a",
        ASSIGNVAR("b", VAR("c"))
    )
);

std::string expressionTest22 = R"(
def check(x):
    if not x == 10:
        print("not ten")
    else:
        print("ten")
)";
std::shared_ptr<ASTNode> expressionTest22Result = CODE(
    FUNC("check", STRARR("x"),
        CODE(
            IF(
                NOT(EQ(VAR("x"), INT(10))),
                CODE(PRINT(STR("not ten"))),
                CODE(PRINT(STR("ten")))
            )
        )
    )
);

std::string expressionTest23 = R"(
def compute(a, b):
    while a <= b:
        a = a + 1
    return a
)";
std::shared_ptr<ASTNode> expressionTest23Result = CODE(
    FUNC("compute", STRARR("a", "b"),
        CODE(
            WHILE(LEQ(VAR("a"), VAR("b")),
                CODE(
                    ASSIGNVAR("a", ADD(VAR("a"), INT(1)))
                )
            ),
            RETURN(VAR("a"))
        )
    )
);

std::string expressionTest24 = R"(
def test(x):
    if x * 2 == 10 or x + 3 == 8:
        print("valid")
)";
std::shared_ptr<ASTNode> expressionTest24Result = CODE(
    FUNC("test", STRARR("x"),
        CODE(
            IF(
                OR(
                    EQ(MULT(VAR("x"), INT(2)), INT(10)),
                    EQ(ADD(VAR("x"), INT(3)), INT(8))
                ),
                CODE(PRINT(STR("valid"))), CODE()
            )
        )
    )
);

std::string expressionTest25 = R"(
def logic(a, b, c):
    return not a or b and c
)";

std::shared_ptr<ASTNode> expressionTest25Result = CODE(
    FUNC("logic", STRARR("a", "b", "c"),
        CODE(
            RETURN(
                NOT(
                    OR(VAR("a"), AND(VAR("b"), VAR("c")))
                )
            )
        )
    )
);

std::string expressionTest26 = R"(
def nested():
    def inner():
        while 1:
            if 2 + 2 == 4:
                return "ok"
    return inner
)";
std::shared_ptr<ASTNode> expressionTest26Result = CODE(
    FUNC("nested", STRARR(),
        CODE(
            FUNC("inner", STRARR(),
                CODE(
                    WHILE(INT(1),
                        CODE(
                            IF(
                                EQ(ADD(INT(2), INT(2)), INT(4)),
                                CODE(RETURN(STR("ok"))), CODE()
                            )
                        )
                    )
                )
            ),
            RETURN(VAR("inner"))
        )
    )
);

#endif // TESTS_PARSER_SNIPPETS_HPP
#endif // KRAIT_TESTING