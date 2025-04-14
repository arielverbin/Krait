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

#include "shortcuts.hpp"

using namespace semantics;
using namespace core;

std::vector<std::shared_ptr<ASTNode>> testCommands1 = {
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
};

std::vector<std::shared_ptr<ASTNode>> testCommands2 = {
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

};



#endif // TESTS_COMMANDS_HPP