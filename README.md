# 🐍 Krait

Krait (inspired by Python シ) is an interpreted, dynamically-typed language implemented in C++.

### Project Structure

```text
Krait/
├── src/
│   ├── core/          # Built-in object types (Integer, String, Boolean, etc.)
│   ├── semantics/     # Executable semantics (Variable, Assign, If, While, etc.
│   ├── lexer/         # Split the source code into tokens.
│   ├── parser/        # Parsing the tokens to create the AST's
│   ├── interpreter/   # The interpreter for executing the AST's
│   ├── runtime/       # The state of the program at runtime
│   ├── exceptions/    # Built-in exception classes
│   └── utils/         # Utilities
├── run/               # The tokenize-parse-interpret flow
└── tests/             # Tests
```

### Getting Started
```bash
git clone https://github.com/arielverbin/Krait.git
cd krait
make test  # For running tests/*/tests.cpp
make test parser interpreter # For compiling and running tests/parser/tests.cpp and then tests/interpreter/tests.cpp
make run   # For compiling and running a REPL shell
```
After typing `make run`, you can also use `./build/run/krait path/to/file.kr` to run scripts.

### Read-Evaluate-Print-Loop (RELP)
By running `./build/run/krait` without arguments, a REPL shell will open. Use `exit` to terminate.

<p align="center">
  <img src="./assets/repl-demo.png" alt="REPL demo" width="400"/>
</p>

## TODO
- [x] Member lookup (`obj.field`)
- [ ] Backtrace for runtime errors
- [ ] `list` datatype
- [ ] `for` loops, `in` keyword
- [ ] Support for user-defined types (classes)
