//****************************************************
// Starter code for a symbol table definition.
// You are not required to use any of these names, but you will need the 
// described functionality
//

#pragma once

#include <memory>
#include <string>

#include "cSymbol.h"

using std::string;
using std::unique_ptr;

// NOTE: The following typedef will have to be replaced by something meaningful
typedef void symbolTable_t;

class cSymbolTable
{
    public:
        // Construct an empty symbol table
        cSymbolTable();
        ~cSymbolTable();

        // Increase the scope: add a level to the nested symbol table
        // Return value is the newly created scope
        symbolTable_t *IncreaseScope();

        // Decrease the scope: remove the outer-most scope.
        // Returned value is the outer-most scope AFTER the pop.
        //
        // NOTE: do NOT clean up memory after poping the table. Parts of the
        // AST will probably contain pointers to symbols in the popped table.
        symbolTable_t *DecreaseScope();

        // insert a symbol into the table
        // Assumes the symbol is not already in the table
        void Insert(cSymbol *sym);

        // Do a lookup in the nested table. 
        // NOTE: This starts at the inner-most scope and works its way out until
        // a match is found.
        // Return the symbol for the inner-most match. 
        // Returns nullptr if no match is found.
        cSymbol *Find(string name);

        // Find a symbol in the inner-most scope.
        // NOTE: This ONLY searches the inner-most scope.
        // Returns nullptr if the symbol is not found.
        cSymbol *FindLocal(string name);

    private:
        struct Scope;
        unique_ptr<Scope> m_rootScope;
        Scope *m_currentScope;
};

// declare the global symbol table. The definition will have to be in a cpp file
extern cSymbolTable g_symbolTable;
