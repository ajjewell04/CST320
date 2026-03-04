//**********************************************
// Various utilities that might be useful for a top-down recursive-descent p
// parser
//
// Author: Phil Howard
//
// See utils.h for more documentation

#include <iostream>
#include <string.h>
#include "lex.h"
#include "utils.h"

using std::cout;

static int g_token = -1;

//*******************************************
void Error(std::string expecting)
{
    if (yytext != NULL && strlen(yytext) > 0)
        cout << "Found '" << yytext << "' when expecting a " << expecting;
    else
        cout << "Found End-Of-File when expecting a " << expecting;

    std::cout << " in line " << yylineno << std::endl;
}
//*******************************************
int GetToken()
{
    int token = g_token;
    if (token < 0) token = yylex();
    g_token = -1;

    return token;
}
//*******************************************
int UngetToken(int token)
{
    g_token = token;

    return g_token;
}

//*******************************************
int PeekToken()
{
    if (g_token < 0) g_token = yylex();

    return g_token;
}
//*******************************************
int AdvanceToken()
{
    g_token = yylex();

    return g_token;
}
