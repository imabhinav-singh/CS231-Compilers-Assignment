/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CHAR = 258,
    INT = 259,
    FLOAT = 260,
    DOUBLE = 261,
    IF = 262,
    ELSE = 263,
    WHILE = 264,
    FOR = 265,
    CONTINUE = 266,
    BREAK = 267,
    VOID = 268,
    RETURN = 269,
    BOOL = 270,
    DO = 271,
    THEN = 272,
    ADDOP = 273,
    MULOP = 274,
    DIVOP = 275,
    INCR = 276,
    OROP = 277,
    ANDOP = 278,
    NOTOP = 279,
    EQUOP = 280,
    RELOP = 281,
    LPAREN = 282,
    RPAREN = 283,
    LBRACK = 284,
    RBRACK = 285,
    LBRACE = 286,
    RBRACE = 287,
    SEMI = 288,
    DOT = 289,
    COMMA = 290,
    ASSIGN = 291,
    REFER = 292,
    ID = 293,
    ICONST = 294,
    FCONST = 295,
    CCONST = 296,
    STRING = 297
  };
#endif
/* Tokens.  */
#define CHAR 258
#define INT 259
#define FLOAT 260
#define DOUBLE 261
#define IF 262
#define ELSE 263
#define WHILE 264
#define FOR 265
#define CONTINUE 266
#define BREAK 267
#define VOID 268
#define RETURN 269
#define BOOL 270
#define DO 271
#define THEN 272
#define ADDOP 273
#define MULOP 274
#define DIVOP 275
#define INCR 276
#define OROP 277
#define ANDOP 278
#define NOTOP 279
#define EQUOP 280
#define RELOP 281
#define LPAREN 282
#define RPAREN 283
#define LBRACK 284
#define RBRACK 285
#define LBRACE 286
#define RBRACE 287
#define SEMI 288
#define DOT 289
#define COMMA 290
#define ASSIGN 291
#define REFER 292
#define ID 293
#define ICONST 294
#define FCONST 295
#define CCONST 296
#define STRING 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 11 "parser.y"

    char *str;

#line 145 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
