%{
    #include<stdio.h>
    #include<string.h>
    #include<stdlib.h>
    #include<math.h>
    #include "main.tab.h"

    extern int yylineno;      // Flex built-in line tracker
    FILE *token_fp;           // Token file pointer
    extern YYSTYPE yylval;    // Added this line to access union type

    void open_token_file() {
        token_fp = fopen("Output/tokens.txt", "w");
        if (!token_fp) {
            printf("Error opening token file.\n");
            exit(1);
        }
    }

    void close_token_file() {
        if (token_fp)
            fclose(token_fp);
    }

    void write_token(const char *token_name, const char *lexeme) {
        fprintf(token_fp, "Line %d: Token: %-20s Lexeme: %s\n", yylineno, token_name, lexeme);
    }
%}

%option yylineno
%option noyywrap

variable [a-zA-Z]
digit [0-9]
header		"import ".*
Comment		#.*

%%

"YU_CH_MYAR_MATTER" 			{ write_token("CASE", yytext); return CASE; }
"PALATI_JOL" 					{ write_token("SWITCH", yytext); return SWITCH; }
"KWI_MATTER_NI_RAYN_MYAR" 		{ write_token("DEFAULT", yytext); return DEFAULT; }
{digit}+						{ write_token("NUM", yytext); yylval.num = atoi(yytext); return NUM; }
"LIKHNU_CHAUN_ME" 				{ write_token("PRINTFUNCTION", yytext); return PRINTFUNCTION; } 
"YADI" 			   				{ write_token("IF", yytext); return IF; }
"NITHAR" 		   				{ write_token("ELSE", yytext); return ELSE; }
"{" 		       				{ write_token("BRACKETSTART", yytext); return BRACKETSTART; }
"}"  		       				{ write_token("BRACKETEND", yytext); return BRACKETEND; }
"GOL_CHAKARI_KU_BHULA" 			{ write_token("FOR", yytext); return FOR; }
"GOL_CHAKARI_KI_BHULI"			{ write_token("WHILE", yytext); return WHILE; }
"LAMBUGUNA"						{ write_token("FACTORIAL", yytext); return FACTORIAL; }
"DWI_BATI_BHAG_HUNU_KI_NA"		{ write_token("ODDEVEN", yytext); return ODDEVEN; }
"SIN"							{ write_token("SIN", yytext); return SIN; }
"COS"          					{ write_token("COS", yytext); return COS; }
"TAN"          					{ write_token("TAN", yytext); return TAN; }
"LOG"							{ write_token("LOG", yytext); return LOG; }
"ANK"  							{ write_token("INT", yytext); return INT; }
"AKSHAR" 						{ write_token("CHAR", yytext); return CHAR; }
"DASHAMLABH"			 		{ write_token("FLOAT", yytext); return FLOAT; }
"CHIJO_KU_GODAM"				{ write_token("ARRAY", yytext); return ARRAY; }
"TRY"							{ write_token("TRY", yytext); return TRY; }
"CATCH"							{ write_token("CATCH", yytext); return CATCH; }
"CHAKRACHAL"					{ write_token("FUNCTION", yytext); return FUNCTION; }
"CLASS"							{ write_token("CLASS", yytext); return CLASS; }
"SHURUAT_KANU_CHAUN"			{ write_token("MAIN", yytext); return MAIN; }
{Comment}						{ write_token("COMMENT", yytext); /* ignored */ }
{header}  						{ write_token("HEADER", yytext); /* ignored */ }
[-+/*<>=,():;%^]     			{ char op[2] = {yytext[0], '\0'}; write_token("OPERATOR", op); return *yytext; }
{variable}	    				{ char var[2] = {yytext[0], '\0'}; write_token("VAR", var); yylval.var = yytext[0]; return VAR; }

[ \t\r]+						{ /* ignore spaces, tabs */ }
\n								{ /* line counted automatically */ }
.								{ write_token("UNKNOWN", yytext); fprintf(stderr, "Unknown Syntax at Line %d: %s\n", yylineno, yytext); }

%%