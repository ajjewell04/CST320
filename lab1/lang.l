%{
//**************************************
// lang.l
//
// Scanner definition file
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

// Illustration showing how to use {} to control scope
// NOTE: this is pseudo-code illustrating how you can adapt your current lang.l

#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "tokens.h"

static int ProcessID();
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

    /* definitions go here. whitespace is an example. */
whitespace      '[\t\n\r ]'

%%
{whitespace}+               {} /* Ignore whitespace */
;                           {} /* Ignore semicolons */
    /* control scope with curly braces
     NOTE: This functionality will eventually move to the parser
    */
"{"                         return OPEN;
"}"                         return CLOSE;
LOCAL                       return LOCAL;
GLOBAL                      return GLOBAL;
LOOKUP                      return LOOKUP;
INSERT                      return INSERT;
[a-z]+                      {
                                return ProcessID();
                            }

%%

//******************************************************
// ProcessID: create symbols in symbol table
static int ProcessID()
{
    string name(yytext);
    cSymbol *sym;

    if (g_insert)
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
        sym = new cSymbol(name);
    }
    yylval.symbol = sym;

    return IDENTIFIER;
}
