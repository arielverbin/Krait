#include <iostream>
#include <vector>

#include "core/Integer.hpp"
#include "core/String.hpp"

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

int main() {    
    std::vector<std::shared_ptr<ASTNode>> commands = {

        // myvar = 10
        std::make_shared<Assign>("myvar", std::make_shared<Const>(std::make_shared<Integer>(10))), 

        // myvar2 = myvar
        std::make_shared<Assign>("myvar2", std::make_shared<Variable>("myvar")),
        
        // myvar = 6
        std::make_shared<Assign>("myvar", std::make_shared<Const>(std::make_shared<Integer>(6))), 

        // print myvar + (myvar2 + 10)
        std::make_shared<Print>(std::make_shared<Sum>(std::make_shared<Variable>(Variable("myvar")),
                                                      std::make_shared<Sum>(std::make_shared<Variable>(Variable("myvar2")), std::make_shared<Const>(std::make_shared<Integer>(10))))),

        // x = "hello, "
        std::make_shared<Assign>("x", std::make_shared<Const>(std::make_shared<String>("hello, "))),

        // y = " world!"
        std::make_shared<Assign>("y", std::make_shared<Const>(std::make_shared<String>("world!"))),

        // print x + y
        std::make_shared<Print>(std::make_shared<Sum>(std::make_shared<Variable>(Variable("x")),
                                                      std::make_shared<Variable>(Variable("y")))),

    };

    Interpreter i(commands);
    i.interpret();
    
    return 0;
}