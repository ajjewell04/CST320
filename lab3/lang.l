%{
#include "lex.h"
#include "tokens.h"
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

whitespace      [ \t\r\n]
comment         \/\/[^\n]*
digit           [0-9]
intconst        {digit}+
ident           [A-Za-z_][A-Za-z0-9_]*

%%
{whitespace}+               {} /* Ignore whitespace */
{comment}                   {} /* Ignore C++ style comments */
"end"                       return END_TOKEN;
{intconst}                  return NUM_TOKEN;
{ident}                     return IDENTIFIER;
"{"                         return '{';
"}"                         return '}';
"("                         return '(';
")"                         return ')';
"["                         return '[';
"]"                         return ']';
";"                         return ';';
"+"                         return '+';
"-"                         return '-';
"*"                         return '*';
"/"                         return '/';
"="                         return '=';
.                           return JUNK_TOKEN;

%%
