%{
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

%}

/* Define the union of semantic values */
%union {
    int num;
    char var;
    struct ASTNode* node;
}

/* Define terminal tokens */
%token NUM VAR IF ELSE FOR WHILE PRINTFUNCTION INT FLOAT CHAR ARRAY
%token BRACKETSTART BRACKETEND FACTORIAL ODDEVEN SIN COS TAN LOG
%token TRY CATCH FUNCTION CLASS MAIN SWITCH CASE DEFAULT

/* Define types for non-terminals */
%type <node> program statements statement block_stmt declaration
%type <node> assign_stmt if_stmt while_stmt for_stmt print_stmt
%type <node> switch_stmt case_stmts case_stmt factorial_stmt
%type <node> oddeven_stmt math_func_stmt expr
%type <num> NUM
%type <var> VAR

%left '+' '-'
%left '*' '/'
%left '<' '>' '='

%%

program: 
    MAIN BRACKETSTART statements BRACKETEND {
        fprintf(parser_output, "Valid program syntax.\n");
        ast_root = create_node(PROGRAM_NODE, 0, 0);
        ast_root->left = $3;
        
        // Perform semantic analysis
        semantic_analysis(ast_root);
        
        // Print AST
        fprintf(ast_output, "Abstract Syntax Tree:\n");
        print_ast(ast_root, 0);
        
        // Generate intermediate code
        fprintf(icg_output, "Intermediate Code:\n");
        generate_icg(ast_root);
    }
    ;

statements:
    statement { $$ = $1; }
    | statement statements { 
        $$ = $1;
        ASTNode *current = $$;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = $2;
    }
    | { $$ = NULL; }
    ;

statement:
    assign_stmt ';' { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    | print_stmt ';' { $$ = $1; }
    | declaration ';' { $$ = $1; }
    | switch_stmt { $$ = $1; }
    | factorial_stmt ';' { $$ = $1; }
    | oddeven_stmt ';' { $$ = $1; }
    | math_func_stmt ';' { $$ = $1; }
    | block_stmt { $$ = $1; }
    ;

block_stmt:
    BRACKETSTART statements BRACKETEND {
        $$ = create_node(BLOCK_NODE, 0, 0);
        $$->left = $2;
    }
    ;

declaration:
    INT VAR {
        add_symbol($2, 0, yylineno); // 0 = INT
        $$ = create_node(VAR_DECL_NODE, 0, $2);
    }
    | FLOAT VAR {
        add_symbol($2, 1, yylineno); // 1 = FLOAT
        $$ = create_node(VAR_DECL_NODE, 1, $2);
    }
    | CHAR VAR {
        add_symbol($2, 2, yylineno); // 2 = CHAR
        $$ = create_node(VAR_DECL_NODE, 2, $2);
    }
    | ARRAY VAR {
        add_symbol($2, 3, yylineno); // 3 = ARRAY
        $$ = create_node(VAR_DECL_NODE, 3, $2);
    }
    ;

assign_stmt:
    VAR '=' expr {
        $$ = create_node(ASSIGN_NODE, 0, $1);
        $$->right = $3;
    }
    ;

if_stmt:
    IF '(' expr ')' block_stmt {
        $$ = create_node(IF_NODE, 0, 0);
        $$->left = $3;
        $$->right = $5;
    }
    | IF '(' expr ')' block_stmt ELSE block_stmt {
        $$ = create_node(IF_NODE, 0, 0);
        $$->left = $3;
        $$->right = $5;
        
        ASTNode *else_node = create_node(ELSE_NODE, 0, 0);
        else_node->left = $7;
        $$->next = else_node;
    }
    ;

while_stmt:
    WHILE '(' expr ')' block_stmt {
        $$ = create_node(WHILE_NODE, 0, 0);
        $$->left = $3;
        $$->right = $5;
    }
    ;

for_stmt:
    FOR '(' assign_stmt ';' expr ';' assign_stmt ')' block_stmt {
        $$ = create_node(FOR_NODE, 0, 0);
        $$->left = $3;
        
        ASTNode *cond_node = create_node(EXPR_NODE, 0, 0);
        cond_node->left = $5;
        
        ASTNode *update_node = create_node(EXPR_NODE, 0, 0);
        update_node->left = $7;
        
        cond_node->right = update_node;
        update_node->right = $9;
        
        $$->right = cond_node;
    }
    ;

switch_stmt:
    SWITCH '(' VAR ')' BRACKETSTART case_stmts BRACKETEND {
        $$ = create_node(SWITCH_NODE, 0, 0);
        $$->left = create_node(VAR_NODE, 0, $3);
        $$->right = $6;
    }
    ;

case_stmts:
    case_stmt { $$ = $1; }
    | case_stmt case_stmts { 
        $$ = $1;
        $$->next = $2;
    }
    ;

case_stmt:
    CASE NUM ':' statements {
        $$ = create_node(CASE_NODE, $2, 0);
        $$->left = $4;
    }
    | DEFAULT ':' statements {
        $$ = create_node(DEFAULT_NODE, 0, 0);
        $$->left = $3;
    }
    ;

print_stmt:
    PRINTFUNCTION '(' expr ')' {
        $$ = create_node(PRINT_NODE, 0, 0);
        $$->left = $3;
    }
    ;

factorial_stmt:
    FACTORIAL '(' expr ')' {
        $$ = create_node(FACTORIAL_NODE, 0, 0);
        $$->left = $3;
    }
    ;

oddeven_stmt:
    ODDEVEN '(' expr ')' {
        $$ = create_node(ODDEVEN_NODE, 0, 0);
        $$->left = $3;
    }
    ;

math_func_stmt:
    SIN '(' expr ')' {
        $$ = create_node(MATH_FUNC_NODE, 's', 0); // 's' for sin
        $$->left = $3;
    }
    | COS '(' expr ')' {
        $$ = create_node(MATH_FUNC_NODE, 'c', 0); // 'c' for cos
        $$->left = $3;
    }
    | TAN '(' expr ')' {
        $$ = create_node(MATH_FUNC_NODE, 't', 0); // 't' for tan
        $$->left = $3;
    }
    | LOG '(' expr ')' {
        $$ = create_node(MATH_FUNC_NODE, 'l', 0); // 'l' for log
        $$->left = $3;
    }
    ;

expr:
    NUM { 
        $$ = create_node(NUM_NODE, $1, 0);
    }
    | VAR { 
        check_initialization($1, yylineno);
        $$ = create_node(VAR_NODE, 0, $1);
    }
    | expr '+' expr { 
        $$ = create_node(EXPR_NODE, '+', 0);
        $$->left = $1;
        $$->right = $3;
    }
    | expr '-' expr { 
        $$ = create_node(EXPR_NODE, '-', 0);
        $$->left = $1;
        $$->right = $3;
    }
    | expr '*' expr { 
        $$ = create_node(EXPR_NODE, '*', 0);
        $$->left = $1;
        $$->right = $3;
    }
    | expr '/' expr { 
        $$ = create_node(EXPR_NODE, '/', 0);
        $$->left = $1;
        $$->right = $3;
    }
    | expr '<' expr { 
        $$ = create_node(EXPR_NODE, '<', 0);
        $$->left = $1;
        $$->right = $3;
    }
    | expr '>' expr { 
        $$ = create_node(EXPR_NODE, '>', 0);
        $$->left = $1;
        $$->right = $3;
    }
    | '(' expr ')' { 
        $$ = $2;
    }
    ;

%%

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