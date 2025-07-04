/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUM = 258,                     /* NUM  */
    VAR = 259,                     /* VAR  */
    IF = 260,                      /* IF  */
    ELSE = 261,                    /* ELSE  */
    FOR = 262,                     /* FOR  */
    WHILE = 263,                   /* WHILE  */
    PRINTFUNCTION = 264,           /* PRINTFUNCTION  */
    INT = 265,                     /* INT  */
    FLOAT = 266,                   /* FLOAT  */
    CHAR = 267,                    /* CHAR  */
    ARRAY = 268,                   /* ARRAY  */
    BRACKETSTART = 269,            /* BRACKETSTART  */
    BRACKETEND = 270,              /* BRACKETEND  */
    FACTORIAL = 271,               /* FACTORIAL  */
    ODDEVEN = 272,                 /* ODDEVEN  */
    SIN = 273,                     /* SIN  */
    COS = 274,                     /* COS  */
    TAN = 275,                     /* TAN  */
    LOG = 276,                     /* LOG  */
    TRY = 277,                     /* TRY  */
    CATCH = 278,                   /* CATCH  */
    FUNCTION = 279,                /* FUNCTION  */
    CLASS = 280,                   /* CLASS  */
    MAIN = 281,                    /* MAIN  */
    SWITCH = 282,                  /* SWITCH  */
    CASE = 283,                    /* CASE  */
    DEFAULT = 284                  /* DEFAULT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 437 "parser.y"

    int num;
    char var;
    struct ASTNode* node;

#line 99 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
