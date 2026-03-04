//**************************************
// main.cpp
//
// Main function for lang compiler
//
// Author: Phil Howard 
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "langparse.h"

// define global variables
long long cSymbol::nextId;

static void InsertBuiltinType(const std::string &name)
{
    cSymbol *symbol = new cSymbol(name);
    symbol->SetIsType(true);
    g_symbolTable.Insert(symbol);
}

// takes two string args: input_file, and output_file
int main(int argc, char **argv)
{
    std::cout << "Anthony Jewell" << std::endl;

    const char *outfile_name;
    int result = 0;

    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == nullptr)
        {
            std::cerr << "ERROR: Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    // Setup the output. If empty, use stdout (which may be redirected)
    if (argc > 2)
    {
        outfile_name = argv[2];

        FILE *output = fopen(outfile_name, "w");
        if (output == nullptr)
        {
            std::cerr << "Unable to open output file " << outfile_name << "\n";
            exit(-1);
        }

        // redirect stdout to the output file
        int output_fd = fileno(output);
        if (dup2(output_fd, 1) != 1)
        {
            std::cerr << "Unable configure output stream\n";
            exit(-1);
        }
    }

    InsertBuiltinType("char");
    InsertBuiltinType("int");
    InsertBuiltinType("float");
    InsertBuiltinType("long");
    InsertBuiltinType("double");

    result = yyparse();
    if (yyast_root != nullptr)
    {
        if (result == 0)
        {
            std::cout << yyast_root->ToString();
        } else {
            std::cout << " Errors in compile\n";
        }
    }

    if (result == 0 && yylex() != 0)
    {
        std::cout << "Junk at end of program\n";
    }

    return result;
}
