#include <iostream>
#include <vector>

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

#include "interpreter/Interpreter.hpp"
#include "runtime/Environment.hpp"

#include "shortcuts.hpp"

using namespace semantics;
using namespace core;
using namespace interpreter;

int main() {   
    std::vector<std::shared_ptr<ASTNode>> commands = {
        ASSIGNVAR("currentNumber", INT(0)),
        ASSIGNVAR("limit", INT(10)),
        ASSIGNVAR("reachedEnd", BOOL(false)),
        
        FUNC("myFunction", STRARR("arg1", "arg2"), 
            CODE(
                ASSIGNVAR("result", ADD(VAR("arg1"), VAR("arg2"))),
                ASSIGNVAR("final", ADD(VAR("result"), VAR("limit"))),
                PRINT(ADD(STR("final is "), VAR("final")))
            )
        ),

        // while not (currentNumber >= limit):
        WHILE(NOT(GEQ(VAR("currentNumber"), VAR("limit"))),
            CODE(
                // currentLine = "currentNumber=" + currentNumber
                ASSIGNVAR("currentLine", ADD(STR("currentNumber is "), VAR("currentNumber"))),
                PRINT(VAR("currentLine")),
                
                IF(EQ(VAR("currentLine"), STR("currentNumber is 5")), 
                    PRINT(STR("Reached 5!")), 
                    PASS()
                ),
                
                CALL(VAR("myFunction"), ARGS(VAR("currentNumber"), ADD(INT(5), INT(5)))),

                // reachedEnd = (currentNumber == limit - 1)
                ASSIGNVAR("reachedEnd", EQ(VAR("currentNumber"), SUB(VAR("limit"), INT(1)))),
                
                PRINT(ADD(STR("reachedEnd="), VAR("reachedEnd"))),
                PRINT(MULT(STR("-"), INT(5))),

                // currentNumber = currentNumber + 1
                ASSIGNVAR("currentNumber", ADD(VAR("currentNumber"), INT(1)))
            )
        )
        
    };

    Interpreter i(commands);
    i.interpret();
    
    return 0;
}
