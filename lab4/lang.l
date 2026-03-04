%{
//**************************************
// lang.l
//
// Scanner definition file
//
// Author: Anthony Jewell
//

#include <string>
#include <cstdlib>

#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "langparse.h"

using std::string;

static int Process_ID(const char *text);
bool g_insertSymbol = false;
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
"!="                        { yylval.int_val = NOT_EQUALS; return NOT_EQUALS; }
"=="                        { yylval.int_val = EQUALS; return EQUALS; }
"&&"                        { yylval.int_val = AND; return AND; }
"||"                        { yylval.int_val = OR; return OR; }
">="                        { yylval.int_val = GE; return GE; }
"<="                        { yylval.int_val = LE; return LE; }
"program"                   return PROGRAM;
"if"                        return IF;
"else"                      return ELSE;
"endif"                     return ENDIF;
"while"                     return WHILE;
"print"                     return PRINT;
"prints"                    return PRINTS;
"char"                      { yylval.symbol = g_symbolTable.Find("char"); return TYPE_ID; }
"int"                       { yylval.symbol = g_symbolTable.Find("int"); return TYPE_ID; }
"long"                      { yylval.symbol = g_symbolTable.Find("long"); return TYPE_ID; }
"float"                     { yylval.symbol = g_symbolTable.Find("float"); return TYPE_ID; }
"double"                    { yylval.symbol = g_symbolTable.Find("double"); return TYPE_ID; }
"struct"                    return STRUCT;
"array"                     return ARRAY;
"return"                    return RETURN;
{floatconst}                { yylval.float_val = atof(yytext); return FLOAT_VAL; }
{intconst}                  { yylval.int_val = atoi(yytext); return INT_VAL; }
{stringlit}                 { yylval.str_val = new string(yytext); return STRING_LIT; }
{ident}                     return Process_ID(yytext);
"{"                         return '{';
"}"                         return '}';
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
    cSymbol *sym = nullptr;

    if (g_insertSymbol)
    {
        sym = g_symbolTable.FindLocal(name);
        if (sym == nullptr)
        {
            sym = new cSymbol(name);
            g_symbolTable.Insert(sym);
        }
    }
    else
    {
        sym = g_symbolTable.Find(name);
        if (sym == nullptr)
        {
            sym = new cSymbol(name);
            g_symbolTable.Insert(sym);
        }
    }
    
    if (sym->IsType())
    {
        yylval.symbol = sym;
        return TYPE_ID;
    }

    yylval.symbol = sym;
    return IDENTIFIER;
}
