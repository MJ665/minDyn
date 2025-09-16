# Project: Compiler Backend for Managed Runtimes

**PI Name:** Manas Thakur
**Project Duration:** 6 months

## Overview

This repository contains the source code for "MinDyn", a minimal compiler for a toy dynamically-typed language. It is designed as an educational project for interns to learn about the architecture of modern compilers, especially those involving Just-In-Time (JIT) compilation and feedback-driven optimization.

The compiler demonstrates the full pipeline:
1.  **Lexing:** Source code is turned into tokens.
2.  **Parsing:** Tokens are structured into an Abstract Syntax Tree (AST).
3.  **Execution:**
    *   **Interpreter:** A simple, slow tree-walking interpreter executes the AST directly.
    *   **JIT Compiler:** Hot functions (those called frequently) are identified by a profiler and compiled to highly optimized native machine code at runtime using the LLVM framework.

## MinDyn Language

MinDyn is a simple language with functions, variables, basic arithmetic, and if-statements.

**Grammar (BNF):**
```bnf
program     ::= statement*
statement   ::= "def" IDENT "(" paramlist ")" "{" statement* "}" | "return" expr ";" | expr ";" | "if" "(" expr ")" "{" statement* "}" ["else" "{" statement* "}"]
expr        ::= literal | IDENT | IDENT "(" arglist ")" | expr binop expr
literal     ::= NUMBER | STRING
paramlist   ::= /* empty */ | IDENT ("," IDENT)*
arglist     ::= /* empty */ | expr ("," expr)*
binop       ::= "+" | "-" | "*" | "/" | "==" | ">"
