#pragma once
//**************************************
// cVarExprNode.h
//
// Defines an AST node for variable references.
//

#include "cExprNode.h"
#include "cSymbol.h"

class cVarExprNode : public cExprNode
{
    public:
        cVarExprNode(cSymbol *name) : cExprNode()
        {
            AddChild(name);
        }

        void AddPart(cAstNode *part)
        {
            AddChild(part);
        }

        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
