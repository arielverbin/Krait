#include <iostream>
#include <vector>

#include "core/Integer.hpp"
#include "core/String.hpp"
#include "core/None.hpp"

#include "semantics/Assign.hpp"
#include "semantics/Print.hpp"
#include "semantics/Const.hpp"
#include "semantics/Variable.hpp"
#include "semantics/Sum.hpp"
#include "semantics/Sub.hpp"

#include "interpreter/Interpreter.hpp"
#include "runtime/Environment.hpp"

using namespace semantics;
using namespace core;
using namespace interpreter;

#define p(OBJECT) std::make_shared<OBJECT>

int main() {    
    std::vector<std::shared_ptr<ASTNode>> commands = {

        // myvar = 10
        p(Assign)("myvar", p(Const)(p(Integer)(10))), 

        // myvar2 = myvar
        p(Assign)("myvar2", p(Variable)("myvar")),
        
        // myvar = 6
        p(Assign)("myvar", p(Const)(p(Integer)(6))), 

        // print myvar + (myvar2 + 10)
        p(Print)(p(Sum)(p(Variable)("myvar"), p(Sum)(p(Variable)("myvar2"), p(Const)(p(Integer)(10))))),

        // x = "hello, "
        p(Assign)("x", p(Const)(p(Integer)(3))),

        // y = " world!"
        p(Assign)("y", p(Const)(p(Integer)(10))),

        // print x + y
        p(Print)(p(Sum)(p(Variable)("x"), p(Variable)("y"))),

    };

    Interpreter i(commands);
    i.interpret();
    
    return 0;
}