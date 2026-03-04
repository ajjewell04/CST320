#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "langparse.h"
#include "cOwnedNodes.h"
#include "cSemantics.h"

#define LAB5B

long long cSymbol::nextId;

static void InsertBuiltinType(const std::string &name)
{
    cSymbol *symbol = cOwnedNodes::Make<cSymbol>(name);
    g_symbolTable.Insert(symbol);

    int size = 0;
    bool isFloat = false;

    if (name == "char") size = 1;
    else if (name == "int") size = 4;
    else if (name == "long") size = 8;
    else if (name == "float")
    {
        size = 4;
        isFloat = true;
    }
    else if (name == "double")
    {
        size = 8;
        isFloat = true;
    }

    symbol->SetDecl(cOwnedNodes::Make<cBaseTypeNode>(name, size, isFloat));
}

int main(int argc, char **argv)
{
    cOwnedNodes::Reset();
    std::cout << "Anthony Jewell" << std::endl;

    const char *outfile_name;
    FILE *input = nullptr;
    FILE *output = nullptr;
    int result = 0;

    if (argc > 1)
    {
        input = fopen(argv[1], "r");
        yyin = input;
        if (yyin == nullptr)
        {
            std::cerr << "ERROR: Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    if (argc > 2)
    {
        outfile_name = argv[2];

        output = fopen(outfile_name, "w");
        if (output == nullptr)
        {
            std::cerr << "Unable to open output file " << outfile_name << "\n";
            exit(-1);
        }

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
    if (yyast_root != nullptr && result == 0)
    {
#ifdef LAB5B
        cSemantics semantics;
        semantics.VisitAllNodes(yyast_root);
#endif

        result += yynerrs;
        if (result == 0)
        {
            std::cout << yyast_root->ToString();
        }
    }

    if (yynerrs != 0)
    {
        std::cout << yynerrs << " Errors in compile\n";
    }

    if (result == 0 && yylex() != 0)
    {
        std::cout << "Junk at end of program\n";
    }

    yylex_destroy();
    cOwnedNodes::Reset();
    if (input != nullptr) fclose(input);
    if (output != nullptr) fclose(output);

    return result;
}
