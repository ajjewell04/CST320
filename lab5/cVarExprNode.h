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
            if (name->GetDecl() == nullptr)
            {
                SemanticParseError("Symbol " + name->GetName() + " not defined",
                    name->GetLineNumber());
            }

            AddChild(name);
        }

        void AddPart(cAstNode *part)
        {
            AddChild(part);
        }

        cDeclNode *GetType() override
        {
            cDeclNode *decl = GetDecl();
            if (decl == nullptr) return nullptr;

            cDeclNode *type = decl->GetType();
            for (int i = 1; i < NumChildren(); i++)
            {
                if (dynamic_cast<cExprNode*>(GetChild(i)) == nullptr)
                {
                    return nullptr;
                }

                if (type != nullptr && type->IsArray())
                {
                    type = type->GetType();
                }
            }

            return type;
        }
        cDeclNode *GetDecl() override
        {
            return GetNameSymbol()->GetDecl();
        }
        cSymbol *GetNameSymbol()
        {
            return static_cast<cSymbol*>(GetChild(0));
        }
        int GetPartCount()
        {
            return NumChildren() - 1;
        }
        cAstNode *GetPart(int index)
        {
            return GetChild(index + 1);
        }
        virtual string NodeType() { return string("varref"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
