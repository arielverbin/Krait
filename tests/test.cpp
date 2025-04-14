#include <iostream>
#include <vector>

#include "core/Integer.hpp"
#include "core/String.hpp"
#include "core/None.hpp"
#include "core/Boolean.hpp"

#include "semantics/Assign.hpp"
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
        ASSIGNVAR("limit", INT(20)),
        ASSIGNVAR("reachedEnd", BOOL(false)),

        // while not (currentNumber >= limit):
        WHILE(NOT(GEQ(VAR("currentNumber"), VAR("limit"))),
            CODE(
                // currentLine = "currentNumber=" + currentNumber
                ASSIGNVAR("currentLine", ADD(STR("currentNumber is "), VAR("currentNumber"))),
                PRINT(VAR("currentLine")),
                
                IF(EQ(VAR("currentLine"), STR("currentNumber=5")), 
                    PRINT(STR("Reached 5!")), 
                    PASS()
                ),

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
