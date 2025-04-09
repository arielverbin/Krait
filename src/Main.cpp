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
#include "semantics/Sum.hpp"
#include "semantics/Sub.hpp"
#include "semantics/If.hpp"

#include "interpreter/Interpreter.hpp"
#include "runtime/Environment.hpp"

using namespace semantics;
using namespace core;
using namespace interpreter;

#define p(OBJECT) std::make_shared<OBJECT>

int main() {    
    std::vector<std::shared_ptr<ASTNode>> commands = {
        p(Assign)("y", p(Const)(p(Integer)(10))),
        p(If)(p(Const)(p(Boolean)(false)),
            p(Code)( (std::vector<std::shared_ptr<ASTNode>>){ p(Assign)("xoxo", p(Const)(p(Integer)(10))), p(Print)(p(Variable)("xoxo"))} ),
            p(Code)( (std::vector<std::shared_ptr<ASTNode>>){ p(Assign)("aaa", p(Const)(p(Integer)(20))), p(Print)(p(Variable)("aaa"))} )
        ),

        // print x + y
        p(Print)(p(Sum)(p(Variable)("xoxo"), p(Variable)("y"))),

    };

    Interpreter i(commands);
    i.interpret();
    
    return 0;
}