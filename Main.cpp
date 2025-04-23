#include <iostream>
#include "run/Runner.hpp"
#include "run/FileSource.hpp"
#include "run/repl/REPLSource.hpp"

int main(int argc, char* argv[]) {
    std::unique_ptr<run::InputSource> source;
    
    if (argc < 2) {
        source = std::make_unique<run::REPLSource>();
    } else {
        source = std::make_unique<run::FileSource>(argv[1]);
    }
    
    run::Runner runner(*source);
    runner.run();
}