# Genesis-of-compiler-Designing-a-Minimal-Language-and-compiler-with-Core-construct-
A custom-built compiler that translates code written in the Garhwali dialect into intermediate representations. Designed using Lex and Yacc (Bison), this project enables native-language programming while supporting syntax analysis, semantic checks, abstract syntax tree generation, and intermediate code generation.


# 🗣️ Garhwali Dialect Programming Language Compiler

The **Garhwali Compiler** is a language processing tool that enables programming in the **Garhwali dialect**, a regional language spoken in the Indian state of Uttarakhand. This compiler translates high-level Garhwali code into an intermediate representation using classical compiler construction tools like **Lex** and **Yacc (Bison)**.

---

## 🌟 Objective

To promote native-language computing by allowing users to write simple programs in the Garhwali dialect and compile them using standard compiler principles. This project supports a full compilation pipeline including lexical analysis, syntax parsing, semantic validation, and AST generation.

---

## ⚙️ Features

- ✅ **Lexical Analysis** – Tokenizes input using Lex (flex).
- ✅ **Syntax Parsing** – Uses Yacc to build parse trees from valid Garhwali grammar.
- ✅ **Semantic Analysis** – Type checking and rule enforcement.
- ✅ **AST Generation** – Builds an abstract syntax tree for valid programs.
- ✅ **Intermediate Representation** – Translates code to intermediate format for further processing.
- ✅ **Garhwali Keywords** – Native keywords mapped to equivalent C-style operations for better accessibility.

---

## 🔤 Sample Keywords Mapping

| Garhwali     | Equivalent in C |
|--------------|-----------------|
| `likhi`      | `print`         |
| `cha`        | `if`            |
| `nahi_cha`   | `else`          |
| `jabtak`     | `while`         |
| `ank`        | `int`           |
| `kaam`       | `main`          |

*(You can extend this table in your docs with more keywords.)*

---

## 🏗️ Tech Stack

- **Lex (Flex)** – Token generation
- **Yacc (Bison)** – Parser and syntax tree builder
- **C/C++** – Backend glue logic and compiler driver

---

## 🧪 How to Run

1. Compile the Lex and Yacc files:
   ```bash
   lex garhwali.l
   yacc -d garhwali.y
   gcc lex.yy.c y.tab.c -o garhwali_compiler
