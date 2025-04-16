#ifndef TESTS_COMMANDS_HPP
#define TESTS_COMMANDS_HPP

#include "core/Integer.hpp"
#include "core/String.hpp"
#include "core/None.hpp"
#include "core/Boolean.hpp"

#include "semantics/define_semantics/Assign.hpp"
#include "semantics/define_semantics/Function.hpp"
#include "semantics/flow_semantics/Call.hpp"
#include "semantics/Print.hpp"
#include "semantics/Const.hpp"
#include "semantics/Variable.hpp"
#include "semantics/operation_semantics/BinaryOp.hpp"
#include "semantics/operation_semantics/UnaryOp.hpp"
#include "semantics/flow_semantics/If.hpp"
#include "semantics/flow_semantics/While.hpp"
#include "semantics/flow_semantics/Pass.hpp"
#include "semantics/flow_semantics/Code.hpp"
#include "semantics/signal_semantics/Return.hpp"
#include "semantics/signal_semantics/Break.hpp"
#include "semantics/signal_semantics/Continue.hpp"

#include <vector>

#include "utils.hpp"

using namespace semantics;
using namespace core;

std::vector<std::shared_ptr<ASTNode>> normalFlow = {
    ASSIGNVAR("currentNumber", INT(0)),
    ASSIGNVAR("limit", INT(10)),
    ASSIGNVAR("reachedEnd", BOOL(false)),
    
    FUNC("myFunction", STRARR("arg1", "arg2"), 
        CODE(
            ASSIGNVAR("result", ADD(VAR("arg1"), VAR("arg2"))),
            ASSIGNVAR("final", ADD(VAR("result"), VAR("limit"))),
            RETURN(VAR("final"))
        )
    ),

    // while not (currentNumber >= limit):
    WHILE(NOT(GEQ(VAR("currentNumber"), VAR("limit"))),
        CODE(
            // currentNumber = currentNumber + 1
            ASSIGNVAR("currentNumber", ADD(VAR("currentNumber"), INT(1))),

            // currentLine = "currentNumber=" + currentNumber
            ASSIGNVAR("currentLine", ADD(STR("currentNumber is "), VAR("currentNumber"))),
            PRINT(VAR("currentLine")),
            
            IF(EQ(VAR("currentLine"), STR("currentNumber is 3")), 
                CODE(
                    PRINT(STR("Reached 3!")),
                    BREAK()
                ), 
                PASS()
            ),
            
            PRINT(ADD(STR("RES="), CALL(VAR("myFunction"), ARGS(VAR("currentNumber"), ADD(INT(5), INT(5)))))),

            // reachedEnd = (currentNumber == limit - 1)
            ASSIGNVAR("reachedEnd", EQ(VAR("currentNumber"), SUB(VAR("limit"), INT(1)))),
            
            PRINT(ADD(STR("reachedEnd="), VAR("reachedEnd"))),
            PRINT(MULT(STR("-"), INT(5)))

        )
    )

    /** 
     * Expected Output:
     * currentNumber is 1
     * RES=21
     * reachedEnd=False
     * -----
     * currentNumber is 2
     * RES=22
     * reachedEnd=False
     * -----
     * currentNumber is 3
     * Reached 3!
     */
};

std::vector<std::shared_ptr<ASTNode>> functionClosures = {
    ASSIGNVAR("numCalls", INT(0)),
    
    FUNC("getSummer", STRARR("value"), 
        CODE(
            FUNC("myFunction", STRARR("value2"),
                CODE(
                    ASSIGNVAR("numCalls", ADD(VAR("numCalls"), INT(1))),
                    RETURN(ADD(VAR("value"), VAR("value2")))
                )
            ),
            
            PRINT(ADD(STR("Returned Summer with "), VAR("value"))),
            RETURN(VAR("myFunction"))
        )
    ),

    ASSIGNVAR("sumWith3", CALL(VAR("getSummer"), ARGS(INT(3)))),
    PRINT(ADD(STR("sumWith3(5) = "), CALL(VAR("sumWith3"), ARGS(INT(5))))),
    PRINT(ADD(STR("sumWith3(10) = "), CALL(VAR("sumWith3"), ARGS(INT(10))))),

    ASSIGNVAR("sumWith5", CALL(VAR("getSummer"), ARGS(INT(5)))),

    PRINT(ADD(STR("sumWith5(5) = "), CALL(VAR("sumWith5"), ARGS(INT(5))))),
    PRINT(ADD(STR("sumWith5(10) = "), CALL(VAR("sumWith5"), ARGS(INT(10))))),

    PRINT(ADD(STR("TOTAL CALLS: "), VAR("numCalls")))

    /** 
     * Expected Output:
     * Returned Summer with 3
     * sumWith3(5) = 8
     * sumWith3(10) = 13
     * Returned Summer with 5
     * sumWith5(5) = 10
     * sumWith5(10) = 15
     * TOTAL CALLS: 4
     */

};

std::vector<std::shared_ptr<ASTNode>> calculations = {
    ASSIGNVAR("var", INT(1)),
    ASSIGNVAR("var2", INT(4)),
    ASSIGNVAR("var", ADD(VAR("var"), MULT(DIV(VAR("var2"), INT(2)), INT(10)))),
    PRINT(DIV(VAR("var"), INT(3))),
    PRINT(ADD(VAR("var"), INT(5))),
    PRINT(SUB(VAR("var"), INT(5))),
    PRINT(MULT(VAR("var"), INT(5))),
    PRINT(DIV(VAR("var"), INT(5))),
    PRINT(ADD(VAR("var"), VAR("var2")))

    /** 
     * Expected Output:
     * 7
     * 26
     * 16
     * 105
     * 4
     * 25
     */

};

std::vector<std::shared_ptr<ASTNode>> controlFlowTest = {
    // Initialize counter
    ASSIGNVAR("i", INT(0)),
    
    // Loop while i < 10
    WHILE(NOT(GEQ(VAR("i"), INT(10))), 
        CODE(
            // Increment counter at start of loop
            ASSIGNVAR("i", ADD(VAR("i"), INT(1))),
            
            // If i is even, print and then skip the remaining block using CONTINUE
            IF(EQ(MOD(VAR("i"), INT(2)), INT(0)),
                CODE(
                    PRINT(ADD(STR("Even: "), VAR("i"))),
                    CONTINUE()
                ),
                PASS()
            ),
            
            // If i equals 7 (and is odd), print a break message and break out of loop
            IF(EQ(VAR("i"), INT(7)),
                CODE(
                    PRINT(STR("Break at 7")),
                    BREAK()
                ),
                PASS()
            ),
            
            // For odd i (except 7 because of the break), print the value
            PRINT(ADD(STR("Odd: "), VAR("i")))
        )
    )

    /** 
     * Expected Output:
     * Odd: 1
     * Even: 2
     * Odd: 3
     * Even: 4
     * Odd: 5
     * Even: 6
     * Break at 7
     */
};

std::vector<std::shared_ptr<ASTNode>> recursionTest = {
    FUNC("fact", STRARR("n"),
        CODE(
            IF(LEQ(VAR("n"), INT(1)),
                CODE(
                    RETURN(INT(1))
                ),
                CODE(
                    RETURN(MULT(VAR("n"), CALL(VAR("fact"), ARGS(SUB(VAR("n"), INT(1))))))
                )
            )
        )
    ),

    FUNC("fib", STRARR("n"),
        CODE(
            IF(OR(EQ(VAR("n"), INT(0)), EQ(VAR("n"), INT(1))),
                CODE(
                    RETURN(INT(1))
                ),
                CODE(
                    RETURN(ADD(CALL(VAR("fib"), ARGS(SUB(VAR("n"), INT(1)))),
                               CALL(VAR("fib"), ARGS(SUB(VAR("n"), INT(2))))))
                )
            )
        )
    ),

    PRINT(CALL(VAR("fact"), ARGS(INT(5)))),
    PRINT(CALL(VAR("fib"), ARGS(INT(6))))
    
    /** 
     * Expected Output:
     * 120
     * 13
     */
};

std::vector<std::shared_ptr<ASTNode>> logicalOperatorsTest = {
    ASSIGNVAR("a", BOOL(true)),
    ASSIGNVAR("b", BOOL(false)),
    PRINT(AND(VAR("a"), VAR("b"))), // expects false
    PRINT(OR(VAR("a"), VAR("b"))),  // expects true
    PRINT(NOT(VAR("a"))),           // expects false
    PRINT(NOT(VAR("b")))            // expects true
    /** 
     * Expected Output:
     * False
     * True
     * False
     * True
     */
};

std::vector<std::shared_ptr<ASTNode>> nestedClosureTest = {
    FUNC("outer", STRARR("x"),
        CODE(
            ASSIGNVAR("y", ADD(VAR("x"), INT(10))),
            FUNC("inner", STRARR("z"),
                CODE(
                    RETURN(ADD(VAR("y"), VAR("z")))
                )
            ),
            RETURN(VAR("inner"))
        )
    ),
    ASSIGNVAR("adder", CALL(VAR("outer"), ARGS(INT(5)))),
    PRINT(CALL(VAR("adder"), ARGS(INT(3))))
    /** 
     * Expected Output:
     * 18   // Explanation: y = 5 + 10 = 15, then inner(3) returns 15 + 3
     */
};

std::vector<std::shared_ptr<ASTNode>> nestedIfElseTest = {
    ASSIGNVAR("score", INT(75)),
    IF(GEQ(VAR("score"), INT(90)),
        CODE(
           PRINT(STR("Grade: A"))
        ),
        IF(GEQ(VAR("score"), INT(80)),
           CODE(
               PRINT(STR("Grade: B"))
           ),
           IF(GEQ(VAR("score"), INT(70)),
              CODE(
                 PRINT(STR("Grade: C"))
              ),
              CODE(
                 PRINT(STR("Grade: D"))
              )
           )
        )
    )
    /** 
     * Expected Output:
     * Grade: C
     */
};

std::vector<std::shared_ptr<ASTNode>> nestedLoopTest = {
    ASSIGNVAR("i", INT(0)),
    WHILE(LT(VAR("i"), INT(3)),
        CODE(
            ASSIGNVAR("j", INT(0)),
            WHILE(LT(VAR("j"), INT(4)),
                CODE(
                    PRINT(ADD(ADD(ADD(STR("i="), VAR("i")), STR(", j=")), VAR("j"))),
                    ASSIGNVAR("j", ADD(VAR("j"), INT(1))),
                    IF(
                        EQ(VAR("j"), INT(3)),
                        CODE(
                            PRINT(STR("Breaking inner loop at j=3")),
                            BREAK()
                        ),
                        PASS()
                    )
                )
            ),
            ASSIGNVAR("i", ADD(VAR("i"), INT(1)))
        )
    )
    /** 
     * Expected Output:
     * i=0, j=0
     * i=0, j=1
     * i=0, j=2
     * Breaking inner loop at j=3
     * i=1, j=0
     * i=1, j=1
     * i=1, j=2
     * Breaking inner loop at j=3
     * i=2, j=0
     * i=2, j=1
     * i=2, j=2
     * Breaking inner loop at j=3
     */
};

std::vector<std::shared_ptr<ASTNode>> noArgFunctionTest = {
    FUNC("sayHello", STRARR(),
        CODE(
            PRINT(STR("Hello, world!"))
        )
    ),
    CALL(VAR("sayHello"), ARGS())
    /** 
     * Expected Output:
     * Hello, world!
     */
};

#endif // TESTS_COMMANDS_HPP
