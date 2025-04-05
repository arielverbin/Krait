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
#include "runtime/GlobalState.hpp"

using namespace semantics;
using namespace core;
using namespace interpreter;

int main() {
    runtime::GlobalState state;
    state.globalScope().map["builtin"] = std::make_shared<Integer>(Integer(14));
    
    std::vector<std::shared_ptr<semantics::ASTNode>> code = {

        // myvar = 10
        std::make_shared<Assign>("myvar", std::make_shared<Const>(std::make_shared<Integer>(10))), 

        // myvar2 = myvar
        std::make_shared<Assign>("myvar2", std::make_shared<Variable>("myvar")),
        
        // myvar = 6
        std::make_shared<Assign>("myvar", std::make_shared<Const>(std::make_shared<Integer>(6))), 

        // print myvar2
        std::make_shared<Print>(std::make_shared<Variable>(Variable("myvar2"))),

        // print (print myvar)
        std::make_shared<Print>(std::make_shared<Print>(std::make_shared<Variable>(Variable("myvar")))),

        // print builtin
        std::make_shared<Print>(std::make_shared<Variable>(Variable("builtin"))),

        // print myvar - myvar2
        std::make_shared<Print>(std::make_shared<Sub>(std::make_shared<Variable>(Variable("myvar")),
                                                      std::make_shared<Variable>(Variable("myvar2")))),

        // gefen = "is ugly"
        std::make_shared<Assign>("gefen", std::make_shared<Const>(std::make_shared<String>("is ugly"))),

        // xoxo = " hahaha"
        std::make_shared<Assign>("xoxo", std::make_shared<Const>(std::make_shared<String>(" hahaha"))),

        // print gefen + xoxo
        std::make_shared<Print>(std::make_shared<Sum>(std::make_shared<Variable>(Variable("gefen")),
                                                      std::make_shared<Variable>(Variable("xoxo")))),

    };

    Interpreter::interpret(state, code);
    
    return 0;
}