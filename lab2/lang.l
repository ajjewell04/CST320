%{
//**************************************
// lang.l
//
// Scanner definition file
//
// Author: Anthony Jewell
//

#include <string>

#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "tokens.h"

using std::string;

static int Process_ID(const char *text);
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

whitespace      [ \t\r\n]
comment         \/\/[^\n]*
digit           [0-9]
intconst        {digit}+
floatconst      {digit}+"."{digit}+
ident           [A-Za-z_][A-Za-z0-9_]*
stringlit       \"[^\"\n]*\"

%%
{whitespace}+               {} /* Ignore whitespace */
{comment}                   {} /* Ignore C++ style comments */
"!="                        return NOT_EQUALS;
"=="                        return EQUALS;
"&&"                        return AND;
"||"                        return OR;
">="                        return GE;
"<="                        return LE;
"program"                   return PROGRAM;
"if"                        return IF;
"else"                      return ELSE;
"endif"                     return ENDIF;
"while"                     return WHILE;
"print"                     return PRINT;
"prints"                    return PRINTS;
"char"                      return CHAR;
"int"                       return INT;
"long"                      return LONG;
"float"                     return FLOAT;
"double"                    return DOUBLE;
"struct"                    return STRUCT;
"array"                     return ARRAY;
"return"                    return RETURN;
{floatconst}                return FLOAT_VAL;
{intconst}                  return INT_VAL;
{stringlit}                 return STRING_LIT;
{ident}                     return Process_ID(yytext);
"{"                         { g_symbolTable.IncreaseScope(); return '{'; }
"}"                         { g_symbolTable.DecreaseScope(); return '}'; }
"("                         return '(';
")"                         return ')';
"["                         return '[';
"]"                         return ']';
";"                         return ';';
","                         return ',';
"."                         return '.';
"+"                         return '+';
"-"                         return '-';
"*"                         return '*';
"/"                         return '/';
"%"                         return '%';
"="                         return '=';
">"                         return '>';
"<"                         return '<';
.                           return JUNK_TOKEN;

%%

static int Process_ID(const char *text)
{
    string name(text);
    cSymbol *sym = g_symbolTable.FindLocal(name);

    if (sym == nullptr)
    {
        sym = new cSymbol(name);
        g_symbolTable.Insert(sym);
    }

    yylval.symbol = sym;
    return IDENTIFIER;
}
