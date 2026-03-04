//**************************************
// main.cpp
//
// Main routine for lang compiler.
// This version only runs the lexer
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 23, 2015
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "tokens.h"

cSymbolTable g_symbolTable;
long long cSymbol::nextId = 0;
yylval_t yylval;
int g_insert = 1;           // global to indicate that symbols should be 
                            // inserted into the symbol table
int g_local = 0;            // global to indicate to do local lookups

// **************************************************
// argv[1] is the input file
// argv[2] if present, is the output file
int main(int argc, char **argv)
{
    const char *outfile_name;
    int result = 0;
    int token;

    std::cout << "Anthony Jewell" << std::endl;

    // open input
    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL)
        {
            std::cerr << "Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    // open output
    if (argc > 2)
    {
        outfile_name = argv[2];
        FILE *output = fopen(outfile_name, "w");
        if (output == NULL)
        {
            std::cerr << "Unable to open output file " << outfile_name << "\n";
            exit(-1);
        }
        int output_fd = fileno(output);
        if (dup2(output_fd, 1) != 1)
        {
            std::cerr << "Unable to send output to " << outfile_name << "\n";
            exit(-2);
        }
    }

    std::cout << "<program>\n";

    token = yylex();
    while (token != 0)
    {
        // if we found an identifier, print it out
        if (token == IDENTIFIER) 
        {
            cSymbol *sym;
            if (!g_insert)
            {
                if (g_local)
                    sym = g_symbolTable.FindLocal(yylval.symbol->GetName());
                else
                    sym = g_symbolTable.Find(yylval.symbol->GetName());

                if (sym != nullptr) yylval.symbol = sym;
            }

            // this will either be the one found above or the one created
            // in the scanner.
            std::cout << yylval.symbol->ToString() << "\n";
        }
        else if (token == LOCAL)
        {
            std::cout << "<local />\n";
            g_local = 1;
        }
        else if (token == GLOBAL)
        {
            std::cout << "<global />\n";
            g_local = 0;
        }
        else if (token == LOOKUP)
        {
            std::cout << "<lookup />\n";
            g_insert = 0;
        }
        else if (token == INSERT)
        {
            std::cout << "<insert />\n";
            g_insert = 1;
        }
        else if (token == OPEN)
        {
            std::cout << "<open />\n";
            g_symbolTable.IncreaseScope();
        }
        else if (token == CLOSE)
        {
            std::cout << "<close />\n";
            g_symbolTable.DecreaseScope();
        }
        token = yylex();
    }

    std::cout << "</program>\n";

    return result;
}
