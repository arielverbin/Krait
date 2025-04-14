# 🐍 Krait

Krait (inspired by Python シ) is an interpreted, dynamically-typed language implemented in C++.

### Project Structure

```text
Krait/
├── src/
    ├── core/          # Built-in object types (Integer, String, Boolean, etc.)
    ├── semantics/     # Executable semantics (Variable, Assign, If, While, etc.)
    ├── interpreter/   # The interpreter for executing the AST's
    ├── runtime/       # The state of the program at runtime
    ├── parser/        # Parsing logic (planned or WIP)
├── tests/             # Tests
```

### Getting Started
```bash
git clone https://github.com/arielverbin/Krait.git
cd krait
make test  # For running tests/test.cpp
make run   # For running Main.cpp (Empty for now)
```
