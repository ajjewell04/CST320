#include <iostream>
#include "lex.h"
#include "parse.h"
#include "tokens.h"
#include "utils.h"

namespace
{
bool IsExprStart(int token)
{
    return token == '(' || token == '[' || token == NUM_TOKEN || token == '{';
}

bool IsStmtStart(int token)
{
    return token == IDENTIFIER || IsExprStart(token);
}

bool MatchToken(int expected, const std::string &name)
{
    if (PeekToken() != expected)
    {
        Error(name);
        return false;
    }

    GetToken();
    return true;
}

void RecoverStatement()
{
    int token = PeekToken();
    while (token != 0 && token != ';' && token != END_TOKEN)
    {
        GetToken();
        token = PeekToken();
    }

    if (token == ';')
    {
        GetToken();
    }
}
}

//*******************************************
// Find a PROG non-terminal
bool FindPROG()
{
    FindSTMTS();

    if (PeekToken() != END_TOKEN)
    {
        Error("'end'");
        return false;
    }

    GetToken();
    return true;
}
//*******************************************
bool FindSTMTS()
{
    while (true)
    {
        int token = PeekToken();

        if (token == END_TOKEN)
        {
            return true;
        }

        if (token == 0)
        {
            return false;
        }

        if (IsStmtStart(token))
        {
            FindSTMT();
            continue;
        }

        Error("statement or 'end'");
        RecoverStatement();
    }
}
//*******************************************
bool FindSTMT()
{
    int token = PeekToken();

    if (token == IDENTIFIER)
    {
        GetToken();

        if (!MatchToken('=', "'='"))
        {
            RecoverStatement();
            return false;
        }

        if (!FindEXPR())
        {
            RecoverStatement();
            return false;
        }

        if (!MatchToken(';', "';'"))
        {
            RecoverStatement();
            return false;
        }

        std::cout << "Found a statement\n";
        return true;
    }

    if (!IsExprStart(token))
    {
        Error("statement");
        RecoverStatement();
        return false;
    }

    if (!FindEXPR())
    {
        RecoverStatement();
        return false;
    }

    if (!MatchToken(';', "';'"))
    {
        RecoverStatement();
        return false;
    }

    std::cout << "Found a statement\n";
    return true;
}
//*******************************************
bool FindEXPR()
{
    if (PeekToken() == '(')
    {
        GetToken();

        if (!FindEXPR()) return false;
        if (!MatchToken(')', "')'")) return false;

        return FindEXPR_P();
    }

    return FindTERM();
}
//*******************************************
bool FindEXPR_P()
{
    while (PeekToken() == '+' || PeekToken() == '-')
    {
        if (!FindPLUSOP()) return false;
        if (!MatchToken('(', "'('")) return false;
        if (!FindEXPR()) return false;
        if (!MatchToken(')', "')'")) return false;
    }

    return true;
}
//*******************************************
bool FindPLUSOP()
{
    int token = PeekToken();
    if (token != '+' && token != '-')
    {
        Error("'+' or '-'");
        return false;
    }

    GetToken();
    return true;
}
//*******************************************
bool FindTERM()
{
    int token = PeekToken();

    if (token == '[')
    {
        GetToken();

        if (!FindEXPR()) return false;
        if (!MatchToken(']', "']'")) return false;

        return FindTERM_P();
    }

    if (token == NUM_TOKEN)
    {
        GetToken();
        return true;
    }

    if (token == '{')
    {
        GetToken();

        if (!MatchToken(IDENTIFIER, "identifier")) return false;
        if (!MatchToken('}', "'}'")) return false;

        return true;
    }

    Error("term");
    return false;
}
//*******************************************
bool FindTERM_P()
{
    while (PeekToken() == '*' || PeekToken() == '/')
    {
        if (!FindTIMESOP()) return false;
        if (!MatchToken('[', "'['")) return false;
        if (!FindEXPR()) return false;
        if (!MatchToken(']', "']'")) return false;
    }

    return true;
}
//*******************************************
bool FindTIMESOP()
{
    int token = PeekToken();
    if (token != '*' && token != '/')
    {
        Error("'*' or '/'");
        return false;
    }

    GetToken();
    return true;
}
