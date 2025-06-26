# Garhwali Programming Language Compiler
Welcome to the official documentation of the Garhwali Dialect Compiler, a language processing tool that enables you to write code in your native Garhwali language and compile it into meaningful intermediate representations. Inspired by C and built using Lex and Yacc (Bison), this compiler provides a full stack from lexical analysis to syntax and semantic analysis, including GUI support for a more user-friendly experience.
________________________________________
# Introduction
Garhwali is a dialect spoken in the Garhwal region of Uttarakhand, India. This compiler project localizes programming by translating common C syntax and semantics into Garhwali-equivalent constructs. The goal is to make programming more intuitive for native Garhwali speakers, educators, and enthusiasts.
This documentation serves as a complete reference guide for:
•	Understanding Garhwali syntax
•	Learning how it maps to C constructs
•	Building, running, and extending the compiler
________________________________________
# Getting Started
Prerequisites
You should have the following installed:
•	Lex (flex)
•	Yacc (Bison)
•	gcc
Folder Structure
/garhwali-compiler
├── lexer.l             # Lexical Analyzer
├── main.y              # Bison Parser with grammar
├── main.tab.c/.h       # Auto-generated parser files
├── compiler            # Executable compiler file
├── sample_program.gad  # Sample Garhwali source code
├── gadwali             # Custom dialect file (optional)
Build Instructions
lex lexer.l
yacc -d main.y
gcc -o compiler lex.yy.c main.tab.c -lm
./compiler < sample_program.gad
________________________________________
# Language Syntax Reference
Garhwali keywords are mapped to their C equivalents as follows:
Garhwali Keyword	Equivalent in C	Description

ank: int	Integer declaration
pankha:	float	Floating-point number
akshar:	char	Character data type
agar:	if	Conditional statement
nahi:	else	Alternate condition
tabtak:	while	While loop
har:	for	For loop
chhap:	print	Console output
soch:	switch	Switch-case branching
mamla:	case	Case condition
niyam:	default	Default case
sikh:	main	Entry function
karya:	function	User-defined function
wapis:	return	Return statement
kosis:	try	Exception handling (try)
pakad:	catch	Exception handling (catch)
kos:	sin	Sine function
kosin:	cos	Cosine function
tan	tan:	Tangent function
ghaat:	log	Logarithmic function
________________________________________
# Program Structure
Sample Code
ank a = 5;
ank b = 10;
agar (a < b)
{
    chhap a;
}
# Abstract Syntax Tree (AST)
•	Nodes like PROGRAM_NODE, IF_NODE, EXPR_NODE, etc.
•	Supports semantic checks: uninitialized variables, redeclarations
# Symbol Table
Each variable has:
•	Name (e.g., a, b)
•	Type (int, float, etc.)
•	Line of declaration
•	Initialization status
________________________________________
# Compiler Components
# Lexical Analyzer (lexer.l)
Defines tokens:
•	Keywords: ank, agar, chhap, etc.
•	Operators: +, -, *, /, =, ==, <, >
•	Identifiers and numbers
# Parser (main.y)
Implements grammar rules:
•	Statement parsing: declarations, loops, conditionals
•	Expression parsing: arithmetic and relational
•	AST generation
# Semantic Analysis
•	Undeclared variable usage
•	Uninitialized variable access
•	Type checking (basic)
# Graphical User Interface (GUI)
•	Syntax highlighting and error display
•	Tree visualization for AST

  
# File Outputs
Output File	Description
parser_output.txt	Syntax and semantic error logs
ast.txt	Abstract Syntax Tree
symbol_table.txt	Variable info and declarations
________________________________________
# Example Use Cases
# Educational Tools
# Teach programming concepts using regional language syntax.
# Localization
# Bridge the digital divide for non-English-speaking users.
# Native DSLs
# Use Garhwali dialect for cultural computing or traditional systems.
