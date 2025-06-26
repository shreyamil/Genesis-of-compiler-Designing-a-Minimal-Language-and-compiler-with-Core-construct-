/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "main.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// AST node types
typedef enum {
    PROGRAM_NODE,
    IF_NODE,
    ELSE_NODE,
    WHILE_NODE,
    FOR_NODE,
    PRINT_NODE,
    VAR_DECL_NODE,
    ASSIGN_NODE,
    EXPR_NODE,
    NUM_NODE,
    VAR_NODE,
    FUNCTION_NODE,
    CALL_NODE,
    PARAM_NODE,
    RETURN_NODE,
    SWITCH_NODE,
    CASE_NODE,
    DEFAULT_NODE,
    BLOCK_NODE,
    FACTORIAL_NODE,
    ODDEVEN_NODE,
    MATH_FUNC_NODE
} NodeType;

// AST node structure
typedef struct ASTNode {
    NodeType type;
    int value;          // For numeric values
    char var_name;      // For variable names
    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *next;   // For statement lists
} ASTNode;

// Symbol table entry
typedef struct SymbolEntry {
    char name;          // Single-character variable name
    int type;           // 0 = INT, 1 = FLOAT, 2 = CHAR, 3 = ARRAY
    int initialized;    // 0 = not initialized, 1 = initialized
    int line_declared;  // Line number where declared
    struct SymbolEntry *next;
} SymbolEntry;

// Global variables
SymbolEntry *symbol_table = NULL;
ASTNode *ast_root = NULL;
FILE *parser_output;
FILE *icg_output;
FILE *ast_output;
int temp_var_count = 1;
int label_count = 1;

// External declarations
extern int yylex();
extern int yylineno;
extern char* yytext;

// Function declarations
ASTNode* create_node(NodeType type, int value, char var_name);
void add_symbol(char name, int type, int line);
SymbolEntry* find_symbol(char name);
void check_initialization(char name, int line);
void print_ast(ASTNode *node, int indent);
void generate_icg(ASTNode *node);
char* new_temp();
int new_label();
void semantic_analysis(ASTNode *node);
void yyerror(const char *s);

// Function to create a new AST node
ASTNode* create_node(NodeType type, int value, char var_name) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->type = type;
    node->value = value;
    node->var_name = var_name;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;
    return node;
}

// Add a symbol to the symbol table
void add_symbol(char name, int type, int line) {
    // Check if symbol already exists
    SymbolEntry *existing = find_symbol(name);
    if (existing != NULL) {
        fprintf(parser_output, "Semantic Error: Variable '%c' already declared. Line: %d\n", name, line);
        return;
    }
    
    // Create new symbol entry
    SymbolEntry *entry = (SymbolEntry*)malloc(sizeof(SymbolEntry));
    if (entry == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    entry->name = name;
    entry->type = type;
    entry->initialized = 0;
    entry->line_declared = line;
    entry->next = symbol_table;
    symbol_table = entry;
}

// Find a symbol in the symbol table
SymbolEntry* find_symbol(char name) {
    SymbolEntry *current = symbol_table;
    while (current != NULL) {
        if (current->name == name) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Check if a variable is initialized
void check_initialization(char name, int line) {
    SymbolEntry *entry = find_symbol(name);
    if (entry == NULL) {
        fprintf(parser_output, "Semantic Error: Variable '%c' used but not declared. Line: %d\n", name, line);
    } else if (entry->initialized == 0) {
        fprintf(parser_output, "Semantic Warning: Variable '%c' may be used without initialization. Line: %d\n", name, line);
    }
}

// Mark a variable as initialized
void mark_initialized(char name) {
    SymbolEntry *entry = find_symbol(name);
    if (entry != NULL) {
        entry->initialized = 1;
    }
}

// Create a new temporary variable name
char* new_temp() {
    char *temp = (char*)malloc(10);
    sprintf(temp, "t%d", temp_var_count++);
    return temp;
}

// Create a new label
int new_label() {
    return label_count++;
}

// Print the AST to a file
void print_ast(ASTNode *node, int indent) {
    if (node == NULL) return;
    
    // Print indentation
    for (int i = 0; i < indent; i++) {
        fprintf(ast_output, "  ");
    }
    
    // Print node type
    switch (node->type) {
        case PROGRAM_NODE:
            fprintf(ast_output, "PROGRAM\n");
            break;
        case IF_NODE:
            fprintf(ast_output, "IF\n");
            break;
        case ELSE_NODE:
            fprintf(ast_output, "ELSE\n");
            break;
        case WHILE_NODE:
            fprintf(ast_output, "WHILE\n");
            break;
        case FOR_NODE:
            fprintf(ast_output, "FOR\n");
            break;
        case PRINT_NODE:
            fprintf(ast_output, "PRINT\n");
            break;
        case VAR_DECL_NODE:
            fprintf(ast_output, "VAR_DECL: %c\n", node->var_name);
            break;
        case ASSIGN_NODE:
            fprintf(ast_output, "ASSIGN: %c\n", node->var_name);
            break;
        case EXPR_NODE:
            fprintf(ast_output, "EXPR: %c\n", node->value); // Operator
            break;
        case NUM_NODE:
            fprintf(ast_output, "NUM: %d\n", node->value);
            break;
        case VAR_NODE:
            fprintf(ast_output, "VAR: %c\n", node->var_name);
            break;
        case FUNCTION_NODE:
            fprintf(ast_output, "FUNCTION\n");
            break;
        case CALL_NODE:
            fprintf(ast_output, "CALL\n");
            break;
        case PARAM_NODE:
            fprintf(ast_output, "PARAM\n");
            break;
        case RETURN_NODE:
            fprintf(ast_output, "RETURN\n");
            break;
        case SWITCH_NODE:
            fprintf(ast_output, "SWITCH\n");
            break;
        case CASE_NODE:
            fprintf(ast_output, "CASE: %d\n", node->value);
            break;
        case DEFAULT_NODE:
            fprintf(ast_output, "DEFAULT\n");
            break;
        case BLOCK_NODE:
            fprintf(ast_output, "BLOCK\n");
            break;
        case FACTORIAL_NODE:
            fprintf(ast_output, "FACTORIAL\n");
            break;
        case ODDEVEN_NODE:
            fprintf(ast_output, "ODDEVEN\n");
            break;
        case MATH_FUNC_NODE:
            fprintf(ast_output, "MATH_FUNC: %c\n", node->value); // Function type
            break;
        default:
            fprintf(ast_output, "UNKNOWN\n");
    }
    
    // Recursively print children
    print_ast(node->left, indent + 1);
    print_ast(node->right, indent + 1);
    
    // Print next statement
    print_ast(node->next, indent);
}

// Perform semantic analysis on the AST
void semantic_analysis(ASTNode *node) {
    if (node == NULL) return;
    
    // Check different node types
    switch (node->type) {
        case VAR_NODE:
            check_initialization(node->var_name, yylineno);
            break;
        case ASSIGN_NODE:
            // Check if variable exists
            if (find_symbol(node->var_name) == NULL) {
                fprintf(parser_output, "Semantic Error: Variable '%c' assigned but not declared. Line: %d\n", 
                        node->var_name, yylineno);
            } else {
                // Mark as initialized
                mark_initialized(node->var_name);
            }
            break;
        case VAR_DECL_NODE:
            // Variable declaration is handled during parsing
            break;
        // Add more semantic checks for other node types as needed
    }
    
    // Recursively check children
    semantic_analysis(node->left);
    semantic_analysis(node->right);
    
    // Check next statement
    semantic_analysis(node->next);
}

// Generate intermediate code from the AST
void generate_icg(ASTNode *node) {
    if (node == NULL) return;
    
    char *temp1, *temp2;
    int l1, l2, l3;
    
    switch (node->type) {
        case PROGRAM_NODE:
            generate_icg(node->left);
            break;
            
        case NUM_NODE:
            // No code generation for standalone numbers
            break;
            
        case VAR_NODE:
            // No code generation for standalone variables
            break;
            
        case EXPR_NODE:
            // Generate code for expression
            generate_icg(node->left);
            generate_icg(node->right);
            
            temp1 = new_temp();
            
            if (node->left->type == NUM_NODE && node->right->type == NUM_NODE) {
                // Both operands are constants
                fprintf(icg_output, "%s = %d %c %d\n", temp1, node->left->value, 
                        node->value, node->right->value);
            } else if (node->left->type == NUM_NODE) {
                // Left operand is a constant
                fprintf(icg_output, "%s = %d %c %c\n", temp1, node->left->value, 
                        node->value, node->right->var_name);
            } else if (node->right->type == NUM_NODE) {
                // Right operand is a constant
                fprintf(icg_output, "%s = %c %c %d\n", temp1, node->left->var_name, 
                        node->value, node->right->value);
            } else {
                // Both operands are variables
                fprintf(icg_output, "%s = %c %c %c\n", temp1, node->left->var_name, 
                        node->value, node->right->var_name);
            }
            break;
            
        case ASSIGN_NODE:
            // Generate code for right side expression
            generate_icg(node->right);
            
            if (node->right->type == NUM_NODE) {
                // Assigning a constant
                fprintf(icg_output, "%c = %d\n", node->var_name, node->right->value);
            } else if (node->right->type == VAR_NODE) {
                // Assigning a variable
                fprintf(icg_output, "%c = %c\n", node->var_name, node->right->var_name);
            } else {
                // Assigning an expression result
                temp1 = new_temp();
                fprintf(icg_output, "%c = %s\n", node->var_name, temp1);
            }
            break;
            
        case IF_NODE:
            // Generate labels
            l1 = new_label();
            l2 = new_label();
            
            // Generate code for condition
            generate_icg(node->left);
            
            // If false, jump to l1
            fprintf(icg_output, "if_false %c goto L%d\n", node->left->var_name, l1);
            
            // Generate code for if body
            generate_icg(node->right);
            
            // Jump to end of if-else
            fprintf(icg_output, "goto L%d\n", l2);
            
            // Label for else part
            fprintf(icg_output, "L%d:\n", l1);
            
            // Generate code for else part if exists
            if (node->next != NULL && node->next->type == ELSE_NODE) {
                generate_icg(node->next->left);
                node = node->next; // Skip the else node
            }
            
            // Label for end of if-else
            fprintf(icg_output, "L%d:\n", l2);
            break;
            
        case WHILE_NODE:
            // Generate labels
            l1 = new_label();
            l2 = new_label();
            
            // Start of loop
            fprintf(icg_output, "L%d:\n", l1);
            
            // Generate code for condition
            generate_icg(node->left);
            
            // If false, jump to end
            fprintf(icg_output, "if_false %c goto L%d\n", node->left->var_name, l2);
            
            // Generate code for body
            generate_icg(node->right);
            
            // Jump back to condition
            fprintf(icg_output, "goto L%d\n", l1);
            
            // End of loop
            fprintf(icg_output, "L%d:\n", l2);
            break;
            
        case FOR_NODE:
            // Generate labels
            l1 = new_label();
            l2 = new_label();
            
            // Generate initialization code
            generate_icg(node->left);
            
            // Start of loop
            fprintf(icg_output, "L%d:\n", l1);
            
            // Generate condition code
            generate_icg(node->right->left);
            
            // If false, jump to end
            fprintf(icg_output, "if_false %c goto L%d\n", node->right->left->var_name, l2);
            
            // Generate body code
            generate_icg(node->right->right->right);
            
            // Generate update code
            generate_icg(node->right->right->left);
            
            // Jump back to condition
            fprintf(icg_output, "goto L%d\n", l1);
            
            // End of loop
            fprintf(icg_output, "L%d:\n", l2);
            break;
            
        case PRINT_NODE:
            // Generate code for expression to print
            generate_icg(node->left);
            
            if (node->left->type == NUM_NODE) {
                // Printing a constant
                fprintf(icg_output, "print %d\n", node->left->value);
            } else {
                // Printing a variable
                fprintf(icg_output, "print %c\n", node->left->var_name);
            }
            break;
            
        case FACTORIAL_NODE:
            // Generate code for factorial
            generate_icg(node->left);
            
            temp1 = new_temp();
            if (node->left->type == NUM_NODE) {
                fprintf(icg_output, "%s = factorial(%d)\n", temp1, node->left->value);
            } else {
                fprintf(icg_output, "%s = factorial(%c)\n", temp1, node->left->var_name);
            }
            break;
            
        case ODDEVEN_NODE:
            // Generate code for odd/even check
            generate_icg(node->left);
            
            temp1 = new_temp();
            if (node->left->type == NUM_NODE) {
                fprintf(icg_output, "%s = %d %% 2\n", temp1, node->left->value);
            } else {
                fprintf(icg_output, "%s = %c %% 2\n", temp1, node->left->var_name);
            }
            break;
            
        case MATH_FUNC_NODE:
            // Generate code for math function (sin, cos, tan, log)
            generate_icg(node->left);
            
            temp1 = new_temp();
            if (node->left->type == NUM_NODE) {
                switch (node->value) {
                    case 's': // sin
                        fprintf(icg_output, "%s = sin(%d)\n", temp1, node->left->value);
                        break;
                    case 'c': // cos
                        fprintf(icg_output, "%s = cos(%d)\n", temp1, node->left->value);
                        break;
                    case 't': // tan
                        fprintf(icg_output, "%s = tan(%d)\n", temp1, node->left->value);
                        break;
                    case 'l': // log
                        fprintf(icg_output, "%s = log(%d)\n", temp1, node->left->value);
                        break;
                }
            } else {
                switch (node->value) {
                    case 's': // sin
                        fprintf(icg_output, "%s = sin(%c)\n", temp1, node->left->var_name);
                        break;
                    case 'c': // cos
                        fprintf(icg_output, "%s = cos(%c)\n", temp1, node->left->var_name);
                        break;
                    case 't': // tan
                        fprintf(icg_output, "%s = tan(%c)\n", temp1, node->left->var_name);
                        break;
                    case 'l': // log
                        fprintf(icg_output, "%s = log(%c)\n", temp1, node->left->var_name);
                        break;
                }
            }
            break;
            
        case SWITCH_NODE:
            // Generate code for switch-case
            generate_icg(node->left); // Switch expression
            
            // Each case is handled as a series of if-else conditions
            ASTNode *case_node = node->right;
            l1 = new_label(); // End of switch label
            
            while (case_node != NULL) {
                if (case_node->type == CASE_NODE) {
                    l2 = new_label(); // Next case label
                    
                    // Compare switch expression with case value
                    fprintf(icg_output, "if %c != %d goto L%d\n", 
                            node->left->var_name, case_node->value, l2);
                    
                    // Generate code for case body
                    generate_icg(case_node->left);
                    
                    // Jump to end of switch
                    fprintf(icg_output, "goto L%d\n", l1);
                    
                    // Label for next case
                    fprintf(icg_output, "L%d:\n", l2);
                } else if (case_node->type == DEFAULT_NODE) {
                    // Generate code for default case
                    generate_icg(case_node->left);
                }
                
                case_node = case_node->next;
            }
            
            // End of switch
            fprintf(icg_output, "L%d:\n", l1);
            break;
            
        case BLOCK_NODE:
            // Generate code for all statements in the block
            ASTNode *stmt = node->left;
            while (stmt != NULL) {
                generate_icg(stmt);
                stmt = stmt->next;
            }
            break;
            
        // Handle other node types as needed
    }
    
    // Generate code for next statement in the sequence
    if (node->next != NULL && node->type != IF_NODE) {
        generate_icg(node->next);
    }
}


#line 630 "main.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "main.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUM = 3,                        /* NUM  */
  YYSYMBOL_VAR = 4,                        /* VAR  */
  YYSYMBOL_IF = 5,                         /* IF  */
  YYSYMBOL_ELSE = 6,                       /* ELSE  */
  YYSYMBOL_FOR = 7,                        /* FOR  */
  YYSYMBOL_WHILE = 8,                      /* WHILE  */
  YYSYMBOL_PRINTFUNCTION = 9,              /* PRINTFUNCTION  */
  YYSYMBOL_INT = 10,                       /* INT  */
  YYSYMBOL_FLOAT = 11,                     /* FLOAT  */
  YYSYMBOL_CHAR = 12,                      /* CHAR  */
  YYSYMBOL_ARRAY = 13,                     /* ARRAY  */
  YYSYMBOL_BRACKETSTART = 14,              /* BRACKETSTART  */
  YYSYMBOL_BRACKETEND = 15,                /* BRACKETEND  */
  YYSYMBOL_FACTORIAL = 16,                 /* FACTORIAL  */
  YYSYMBOL_ODDEVEN = 17,                   /* ODDEVEN  */
  YYSYMBOL_SIN = 18,                       /* SIN  */
  YYSYMBOL_COS = 19,                       /* COS  */
  YYSYMBOL_TAN = 20,                       /* TAN  */
  YYSYMBOL_LOG = 21,                       /* LOG  */
  YYSYMBOL_TRY = 22,                       /* TRY  */
  YYSYMBOL_CATCH = 23,                     /* CATCH  */
  YYSYMBOL_FUNCTION = 24,                  /* FUNCTION  */
  YYSYMBOL_CLASS = 25,                     /* CLASS  */
  YYSYMBOL_MAIN = 26,                      /* MAIN  */
  YYSYMBOL_SWITCH = 27,                    /* SWITCH  */
  YYSYMBOL_CASE = 28,                      /* CASE  */
  YYSYMBOL_DEFAULT = 29,                   /* DEFAULT  */
  YYSYMBOL_30_ = 30,                       /* '+'  */
  YYSYMBOL_31_ = 31,                       /* '-'  */
  YYSYMBOL_32_ = 32,                       /* '*'  */
  YYSYMBOL_33_ = 33,                       /* '/'  */
  YYSYMBOL_34_ = 34,                       /* '<'  */
  YYSYMBOL_35_ = 35,                       /* '>'  */
  YYSYMBOL_36_ = 36,                       /* '='  */
  YYSYMBOL_37_ = 37,                       /* ';'  */
  YYSYMBOL_38_ = 38,                       /* '('  */
  YYSYMBOL_39_ = 39,                       /* ')'  */
  YYSYMBOL_40_ = 40,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_program = 42,                   /* program  */
  YYSYMBOL_statements = 43,                /* statements  */
  YYSYMBOL_statement = 44,                 /* statement  */
  YYSYMBOL_block_stmt = 45,                /* block_stmt  */
  YYSYMBOL_declaration = 46,               /* declaration  */
  YYSYMBOL_assign_stmt = 47,               /* assign_stmt  */
  YYSYMBOL_if_stmt = 48,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 49,                /* while_stmt  */
  YYSYMBOL_for_stmt = 50,                  /* for_stmt  */
  YYSYMBOL_switch_stmt = 51,               /* switch_stmt  */
  YYSYMBOL_case_stmts = 52,                /* case_stmts  */
  YYSYMBOL_case_stmt = 53,                 /* case_stmt  */
  YYSYMBOL_print_stmt = 54,                /* print_stmt  */
  YYSYMBOL_factorial_stmt = 55,            /* factorial_stmt  */
  YYSYMBOL_oddeven_stmt = 56,              /* oddeven_stmt  */
  YYSYMBOL_math_func_stmt = 57,            /* math_func_stmt  */
  YYSYMBOL_expr = 58                       /* expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   178

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  47
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  122

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      38,    39,    32,    30,     2,    31,     2,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    40,    37,
      34,    36,    35,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   587,   587,   606,   607,   615,   619,   620,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   633,   640,   644,
     648,   652,   659,   666,   671,   683,   691,   709,   717,   718,
     725,   729,   736,   743,   750,   757,   761,   765,   769,   776,
     779,   783,   788,   793,   798,   803,   808,   813
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "VAR", "IF",
  "ELSE", "FOR", "WHILE", "PRINTFUNCTION", "INT", "FLOAT", "CHAR", "ARRAY",
  "BRACKETSTART", "BRACKETEND", "FACTORIAL", "ODDEVEN", "SIN", "COS",
  "TAN", "LOG", "TRY", "CATCH", "FUNCTION", "CLASS", "MAIN", "SWITCH",
  "CASE", "DEFAULT", "'+'", "'-'", "'*'", "'/'", "'<'", "'>'", "'='",
  "';'", "'('", "')'", "':'", "$accept", "program", "statements",
  "statement", "block_stmt", "declaration", "assign_stmt", "if_stmt",
  "while_stmt", "for_stmt", "switch_stmt", "case_stmts", "case_stmt",
  "print_stmt", "factorial_stmt", "oddeven_stmt", "math_func_stmt", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-76)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -23,    11,    27,    47,   -76,    -7,    15,    24,    31,    33,
      35,    68,    69,    77,    47,    44,    53,    54,    61,    64,
      65,    73,    34,    47,   -76,    46,    56,   -76,   -76,   -76,
     -76,    76,    84,    86,    94,     3,     3,    97,     3,     3,
     -76,   -76,   -76,   -76,   107,     3,     3,     3,     3,     3,
       3,   108,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,
     -76,   -76,     3,   143,   -13,    96,     1,    45,   -76,    55,
      75,    85,    95,   105,   115,    93,   125,     3,     3,     3,
       3,     3,     3,   127,     3,   127,   -76,   -76,   -76,   -76,
     -76,   -76,   -76,   128,   -76,    63,    63,   -11,   -11,   -76,
     -76,   137,   135,   -76,     9,   127,    97,   148,   112,   138,
       9,   -76,   122,   123,    47,   -76,   -76,   127,    47,   -76,
     -76,   -76
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     5,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     0,     0,     0,     0,     0,
       0,     0,     0,     3,    16,     0,     0,     7,     8,     9,
      12,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,    19,    20,    21,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     4,    11,     6,    10,    13,    14,    15,
      39,    40,     0,    22,     0,     0,     0,     0,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    32,    33,    34,    35,
      36,    37,    38,     0,    47,    41,    42,    43,    44,    45,
      46,    23,     0,    25,     0,     0,     0,     0,     0,     0,
      28,    24,     0,     0,     5,    27,    29,     0,     5,    31,
      26,    30
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -76,   -76,   -14,   -76,   -75,   -76,   -36,   -76,   -76,   -76,
     -76,    52,   -76,   -76,   -76,   -76,   -76,   -34
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   109,   110,    31,    32,    33,    34,    63
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      44,    65,    64,     1,    66,    67,    60,    61,   101,    53,
     103,    69,    70,    71,    72,    73,    74,    77,    78,    79,
      80,    81,    82,    81,    82,     3,    83,     4,    76,    35,
     111,    77,    78,    79,    80,    81,    82,   107,   108,    40,
      85,    62,   120,    95,    96,    97,    98,    99,   100,    52,
     102,     5,     6,    36,     7,     8,     9,    10,    11,    12,
      13,    14,    37,    15,    16,    17,    18,    19,    20,    38,
     112,    39,    41,    42,    21,    77,    78,    79,    80,    81,
      82,    43,    45,    54,    86,    77,    78,    79,    80,    81,
      82,    46,    47,    55,    87,    79,    80,    81,    82,    48,
     119,     5,    49,    50,   121,    77,    78,    79,    80,    81,
      82,    51,    75,    56,    88,    77,    78,    79,    80,    81,
      82,    57,    68,    58,    89,    77,    78,    79,    80,    81,
      82,    59,    93,    84,    90,    77,    78,    79,    80,    81,
      82,    14,   104,   105,    91,    77,    78,    79,    80,    81,
      82,   113,   114,   115,    92,    77,    78,    79,    80,    81,
      82,   117,   116,   118,    94,    77,    78,    79,    80,    81,
      82,     0,   106,    77,    78,    79,    80,    81,    82
};

static const yytype_int8 yycheck[] =
{
      14,    37,    36,    26,    38,    39,     3,     4,    83,    23,
      85,    45,    46,    47,    48,    49,    50,    30,    31,    32,
      33,    34,    35,    34,    35,    14,    39,     0,    62,    36,
     105,    30,    31,    32,    33,    34,    35,    28,    29,     4,
      39,    38,   117,    77,    78,    79,    80,    81,    82,    15,
      84,     4,     5,    38,     7,     8,     9,    10,    11,    12,
      13,    14,    38,    16,    17,    18,    19,    20,    21,    38,
     106,    38,     4,     4,    27,    30,    31,    32,    33,    34,
      35,     4,    38,    37,    39,    30,    31,    32,    33,    34,
      35,    38,    38,    37,    39,    32,    33,    34,    35,    38,
     114,     4,    38,    38,   118,    30,    31,    32,    33,    34,
      35,    38,     4,    37,    39,    30,    31,    32,    33,    34,
      35,    37,    15,    37,    39,    30,    31,    32,    33,    34,
      35,    37,    39,    37,    39,    30,    31,    32,    33,    34,
      35,    14,    14,     6,    39,    30,    31,    32,    33,    34,
      35,     3,    40,    15,    39,    30,    31,    32,    33,    34,
      35,    39,   110,    40,    39,    30,    31,    32,    33,    34,
      35,    -1,    37,    30,    31,    32,    33,    34,    35
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    26,    42,    14,     0,     4,     5,     7,     8,     9,
      10,    11,    12,    13,    14,    16,    17,    18,    19,    20,
      21,    27,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    54,    55,    56,    57,    36,    38,    38,    38,    38,
       4,     4,     4,     4,    43,    38,    38,    38,    38,    38,
      38,    38,    15,    43,    37,    37,    37,    37,    37,    37,
       3,     4,    38,    58,    58,    47,    58,    58,    15,    58,
      58,    58,    58,    58,    58,     4,    58,    30,    31,    32,
      33,    34,    35,    39,    37,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    58,    58,    58,    58,    58,
      58,    45,    58,    45,    14,     6,    37,    28,    29,    52,
      53,    45,    47,     3,    40,    15,    52,    39,    40,    43,
      45,    43
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    43,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    45,    46,    46,
      46,    46,    47,    48,    48,    49,    50,    51,    52,    52,
      53,    53,    54,    55,    56,    57,    57,    57,    57,    58,
      58,    58,    58,    58,    58,    58,    58,    58
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     4,     1,     2,     0,     2,     1,     1,     1,
       2,     2,     1,     2,     2,     2,     1,     3,     2,     2,
       2,     2,     3,     5,     7,     5,     9,     7,     1,     2,
       4,     3,     4,     4,     4,     4,     4,     4,     4,     1,
       1,     3,     3,     3,     3,     3,     3,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: MAIN BRACKETSTART statements BRACKETEND  */
#line 587 "main.y"
                                            {
        fprintf(parser_output, "Valid program syntax.\n");
        ast_root = create_node(PROGRAM_NODE, 0, 0);
        ast_root->left = (yyvsp[-1].node);
        
        // Perform semantic analysis
        semantic_analysis(ast_root);
        
        // Print AST
        fprintf(ast_output, "Abstract Syntax Tree:\n");
        print_ast(ast_root, 0);
        
        // Generate intermediate code
        fprintf(icg_output, "Intermediate Code:\n");
        generate_icg(ast_root);
    }
#line 1770 "main.tab.c"
    break;

  case 3: /* statements: statement  */
#line 606 "main.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1776 "main.tab.c"
    break;

  case 4: /* statements: statement statements  */
#line 607 "main.y"
                           { 
        (yyval.node) = (yyvsp[-1].node);
        ASTNode *current = (yyval.node);
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = (yyvsp[0].node);
    }
#line 1789 "main.tab.c"
    break;

  case 5: /* statements: %empty  */
#line 615 "main.y"
      { (yyval.node) = NULL; }
#line 1795 "main.tab.c"
    break;

  case 6: /* statement: assign_stmt ';'  */
#line 619 "main.y"
                    { (yyval.node) = (yyvsp[-1].node); }
#line 1801 "main.tab.c"
    break;

  case 7: /* statement: if_stmt  */
#line 620 "main.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1807 "main.tab.c"
    break;

  case 8: /* statement: while_stmt  */
#line 621 "main.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1813 "main.tab.c"
    break;

  case 9: /* statement: for_stmt  */
#line 622 "main.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1819 "main.tab.c"
    break;

  case 10: /* statement: print_stmt ';'  */
#line 623 "main.y"
                     { (yyval.node) = (yyvsp[-1].node); }
#line 1825 "main.tab.c"
    break;

  case 11: /* statement: declaration ';'  */
#line 624 "main.y"
                      { (yyval.node) = (yyvsp[-1].node); }
#line 1831 "main.tab.c"
    break;

  case 12: /* statement: switch_stmt  */
#line 625 "main.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1837 "main.tab.c"
    break;

  case 13: /* statement: factorial_stmt ';'  */
#line 626 "main.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 1843 "main.tab.c"
    break;

  case 14: /* statement: oddeven_stmt ';'  */
#line 627 "main.y"
                       { (yyval.node) = (yyvsp[-1].node); }
#line 1849 "main.tab.c"
    break;

  case 15: /* statement: math_func_stmt ';'  */
#line 628 "main.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 1855 "main.tab.c"
    break;

  case 16: /* statement: block_stmt  */
#line 629 "main.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 1861 "main.tab.c"
    break;

  case 17: /* block_stmt: BRACKETSTART statements BRACKETEND  */
#line 633 "main.y"
                                       {
        (yyval.node) = create_node(BLOCK_NODE, 0, 0);
        (yyval.node)->left = (yyvsp[-1].node);
    }
#line 1870 "main.tab.c"
    break;

  case 18: /* declaration: INT VAR  */
#line 640 "main.y"
            {
        add_symbol((yyvsp[0].var), 0, yylineno); // 0 = INT
        (yyval.node) = create_node(VAR_DECL_NODE, 0, (yyvsp[0].var));
    }
#line 1879 "main.tab.c"
    break;

  case 19: /* declaration: FLOAT VAR  */
#line 644 "main.y"
                {
        add_symbol((yyvsp[0].var), 1, yylineno); // 1 = FLOAT
        (yyval.node) = create_node(VAR_DECL_NODE, 1, (yyvsp[0].var));
    }
#line 1888 "main.tab.c"
    break;

  case 20: /* declaration: CHAR VAR  */
#line 648 "main.y"
               {
        add_symbol((yyvsp[0].var), 2, yylineno); // 2 = CHAR
        (yyval.node) = create_node(VAR_DECL_NODE, 2, (yyvsp[0].var));
    }
#line 1897 "main.tab.c"
    break;

  case 21: /* declaration: ARRAY VAR  */
#line 652 "main.y"
                {
        add_symbol((yyvsp[0].var), 3, yylineno); // 3 = ARRAY
        (yyval.node) = create_node(VAR_DECL_NODE, 3, (yyvsp[0].var));
    }
#line 1906 "main.tab.c"
    break;

  case 22: /* assign_stmt: VAR '=' expr  */
#line 659 "main.y"
                 {
        (yyval.node) = create_node(ASSIGN_NODE, 0, (yyvsp[-2].var));
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1915 "main.tab.c"
    break;

  case 23: /* if_stmt: IF '(' expr ')' block_stmt  */
#line 666 "main.y"
                               {
        (yyval.node) = create_node(IF_NODE, 0, 0);
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1925 "main.tab.c"
    break;

  case 24: /* if_stmt: IF '(' expr ')' block_stmt ELSE block_stmt  */
#line 671 "main.y"
                                                 {
        (yyval.node) = create_node(IF_NODE, 0, 0);
        (yyval.node)->left = (yyvsp[-4].node);
        (yyval.node)->right = (yyvsp[-2].node);
        
        ASTNode *else_node = create_node(ELSE_NODE, 0, 0);
        else_node->left = (yyvsp[0].node);
        (yyval.node)->next = else_node;
    }
#line 1939 "main.tab.c"
    break;

  case 25: /* while_stmt: WHILE '(' expr ')' block_stmt  */
#line 683 "main.y"
                                  {
        (yyval.node) = create_node(WHILE_NODE, 0, 0);
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 1949 "main.tab.c"
    break;

  case 26: /* for_stmt: FOR '(' assign_stmt ';' expr ';' assign_stmt ')' block_stmt  */
#line 691 "main.y"
                                                                {
        (yyval.node) = create_node(FOR_NODE, 0, 0);
        (yyval.node)->left = (yyvsp[-6].node);
        
        ASTNode *cond_node = create_node(EXPR_NODE, 0, 0);
        cond_node->left = (yyvsp[-4].node);
        
        ASTNode *update_node = create_node(EXPR_NODE, 0, 0);
        update_node->left = (yyvsp[-2].node);
        
        cond_node->right = update_node;
        update_node->right = (yyvsp[0].node);
        
        (yyval.node)->right = cond_node;
    }
#line 1969 "main.tab.c"
    break;

  case 27: /* switch_stmt: SWITCH '(' VAR ')' BRACKETSTART case_stmts BRACKETEND  */
#line 709 "main.y"
                                                          {
        (yyval.node) = create_node(SWITCH_NODE, 0, 0);
        (yyval.node)->left = create_node(VAR_NODE, 0, (yyvsp[-4].var));
        (yyval.node)->right = (yyvsp[-1].node);
    }
#line 1979 "main.tab.c"
    break;

  case 28: /* case_stmts: case_stmt  */
#line 717 "main.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 1985 "main.tab.c"
    break;

  case 29: /* case_stmts: case_stmt case_stmts  */
#line 718 "main.y"
                           { 
        (yyval.node) = (yyvsp[-1].node);
        (yyval.node)->next = (yyvsp[0].node);
    }
#line 1994 "main.tab.c"
    break;

  case 30: /* case_stmt: CASE NUM ':' statements  */
#line 725 "main.y"
                            {
        (yyval.node) = create_node(CASE_NODE, (yyvsp[-2].num), 0);
        (yyval.node)->left = (yyvsp[0].node);
    }
#line 2003 "main.tab.c"
    break;

  case 31: /* case_stmt: DEFAULT ':' statements  */
#line 729 "main.y"
                             {
        (yyval.node) = create_node(DEFAULT_NODE, 0, 0);
        (yyval.node)->left = (yyvsp[0].node);
    }
#line 2012 "main.tab.c"
    break;

  case 32: /* print_stmt: PRINTFUNCTION '(' expr ')'  */
#line 736 "main.y"
                               {
        (yyval.node) = create_node(PRINT_NODE, 0, 0);
        (yyval.node)->left = (yyvsp[-1].node);
    }
#line 2021 "main.tab.c"
    break;

  case 33: /* factorial_stmt: FACTORIAL '(' expr ')'  */
#line 743 "main.y"
                           {
        (yyval.node) = create_node(FACTORIAL_NODE, 0, 0);
        (yyval.node)->left = (yyvsp[-1].node);
    }
#line 2030 "main.tab.c"
    break;

  case 34: /* oddeven_stmt: ODDEVEN '(' expr ')'  */
#line 750 "main.y"
                         {
        (yyval.node) = create_node(ODDEVEN_NODE, 0, 0);
        (yyval.node)->left = (yyvsp[-1].node);
    }
#line 2039 "main.tab.c"
    break;

  case 35: /* math_func_stmt: SIN '(' expr ')'  */
#line 757 "main.y"
                     {
        (yyval.node) = create_node(MATH_FUNC_NODE, 's', 0); // 's' for sin
        (yyval.node)->left = (yyvsp[-1].node);
    }
#line 2048 "main.tab.c"
    break;

  case 36: /* math_func_stmt: COS '(' expr ')'  */
#line 761 "main.y"
                       {
        (yyval.node) = create_node(MATH_FUNC_NODE, 'c', 0); // 'c' for cos
        (yyval.node)->left = (yyvsp[-1].node);
    }
#line 2057 "main.tab.c"
    break;

  case 37: /* math_func_stmt: TAN '(' expr ')'  */
#line 765 "main.y"
                       {
        (yyval.node) = create_node(MATH_FUNC_NODE, 't', 0); // 't' for tan
        (yyval.node)->left = (yyvsp[-1].node);
    }
#line 2066 "main.tab.c"
    break;

  case 38: /* math_func_stmt: LOG '(' expr ')'  */
#line 769 "main.y"
                       {
        (yyval.node) = create_node(MATH_FUNC_NODE, 'l', 0); // 'l' for log
        (yyval.node)->left = (yyvsp[-1].node);
    }
#line 2075 "main.tab.c"
    break;

  case 39: /* expr: NUM  */
#line 776 "main.y"
        { 
        (yyval.node) = create_node(NUM_NODE, (yyvsp[0].num), 0);
    }
#line 2083 "main.tab.c"
    break;

  case 40: /* expr: VAR  */
#line 779 "main.y"
          { 
        check_initialization((yyvsp[0].var), yylineno);
        (yyval.node) = create_node(VAR_NODE, 0, (yyvsp[0].var));
    }
#line 2092 "main.tab.c"
    break;

  case 41: /* expr: expr '+' expr  */
#line 783 "main.y"
                    { 
        (yyval.node) = create_node(EXPR_NODE, '+', 0);
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 2102 "main.tab.c"
    break;

  case 42: /* expr: expr '-' expr  */
#line 788 "main.y"
                    { 
        (yyval.node) = create_node(EXPR_NODE, '-', 0);
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 2112 "main.tab.c"
    break;

  case 43: /* expr: expr '*' expr  */
#line 793 "main.y"
                    { 
        (yyval.node) = create_node(EXPR_NODE, '*', 0);
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 2122 "main.tab.c"
    break;

  case 44: /* expr: expr '/' expr  */
#line 798 "main.y"
                    { 
        (yyval.node) = create_node(EXPR_NODE, '/', 0);
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 2132 "main.tab.c"
    break;

  case 45: /* expr: expr '<' expr  */
#line 803 "main.y"
                    { 
        (yyval.node) = create_node(EXPR_NODE, '<', 0);
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 2142 "main.tab.c"
    break;

  case 46: /* expr: expr '>' expr  */
#line 808 "main.y"
                    { 
        (yyval.node) = create_node(EXPR_NODE, '>', 0);
        (yyval.node)->left = (yyvsp[-2].node);
        (yyval.node)->right = (yyvsp[0].node);
    }
#line 2152 "main.tab.c"
    break;

  case 47: /* expr: '(' expr ')'  */
#line 813 "main.y"
                   { 
        (yyval.node) = (yyvsp[-1].node);
    }
#line 2160 "main.tab.c"
    break;


#line 2164 "main.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 818 "main.y"


int main() {
    // Create output directory if it doesn't exist
    system("mkdir -p Output");
    
    // Open output files
    parser_output = fopen("Output/parser_output.txt", "w");
    if (!parser_output) {
        printf("Error opening parser output file.\n");
        return 1;
    }
    
    ast_output = fopen("Output/ast_output.txt", "w");
    if (!ast_output) {
        printf("Error opening AST output file.\n");
        fclose(parser_output);
        return 1;
    }
    
    icg_output = fopen("Output/icg_output.txt", "w");
    if (!icg_output) {
        printf("Error opening ICG output file.\n");
        fclose(parser_output);
        fclose(ast_output);
        return 1;
    }
    
    // Open token file (from your lexer)
    extern void open_token_file();
    open_token_file();
    
    // Parse the input
    yyparse();
    
    // Clean up
    extern void close_token_file();
    close_token_file();
    fclose(parser_output);
    fclose(ast_output);
    fclose(icg_output);
    
    printf("Compilation completed. Check Output directory for results.\n");
    return 0;
}

void yyerror(const char *s) {
    fprintf(parser_output, "Syntax Error: %s on line %d\n", s, yylineno);
}
