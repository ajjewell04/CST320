#pragma once

#include "cExprNode.h"
#include "cParamListNode.h"
#include "cSymbol.h"

class cFuncExprNode : public cExprNode
{
    public:
        cFuncExprNode(cSymbol *name, cParamListNode *params=nullptr) : cExprNode()
        {
            if (name->GetDecl() == nullptr)
            {
                SemanticParseError("Symbol " + name->GetName() + " not defined",
                    name->GetLineNumber());
            }

            AddChild(name);
            if (params != nullptr) AddChild(params);
        }

        cDeclNode *GetType() override
        {
            cDeclNode *decl = GetDecl();
            return decl == nullptr ? nullptr : decl->GetType();
        }
        cDeclNode *GetDecl() override
        {
            return GetNameSymbol()->GetDecl();
        }
        cSymbol *GetNameSymbol()
        {
            return static_cast<cSymbol*>(GetChild(0));
        }
        cParamListNode *GetParams()
        {
            return NumChildren() > 1 ? static_cast<cParamListNode*>(GetChild(1)) : nullptr;
        }
        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
