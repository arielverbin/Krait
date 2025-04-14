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
└── tests/             # Tests
```

### Getting Started
```bash
git clone https://github.com/arielverbin/Krait.git
cd krait
make test  # For running tests/test.cpp
make run   # For running Main.cpp (Empty for now)
```
