#pragma once
//**************************************
// cVarDeclNode.h
//
// Defines an AST node for variable declarations.
//

#include "cDeclNode.h"
#include "cSymbol.h"

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol *type, cSymbol *name) : cDeclNode()
        {
            bool hasError = (name->GetDecl() != nullptr);
            if (hasError)
            {
                SemanticParseError("Symbol " + name->GetName() +
                    " already defined in current scope", name->GetLineNumber());
            }

            if (!hasError) name->SetDecl(this);
            AddChild(type);
            AddChild(name);
        }

        cDeclNode *GetType() override { return GetTypeSymbol()->GetDecl(); }
        string GetName() override { return GetNameSymbol()->GetName(); }
        bool IsVar() override { return true; }
        cSymbol *GetTypeSymbol()
        {
            return static_cast<cSymbol*>(GetChild(0));
        }
        cSymbol *GetNameSymbol()
        {
            return static_cast<cSymbol*>(GetChild(1));
        }
        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
