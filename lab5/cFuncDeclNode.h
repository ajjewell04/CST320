#pragma once

#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cParamsNode.h"
#include "cStmtsNode.h"
#include "cSymbol.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        cFuncDeclNode(cSymbol *type, cSymbol *name, cParamsNode *params=nullptr,
            cDeclsNode *decls=nullptr, cStmtsNode *stmts=nullptr) : cDeclNode()
        {
            cDeclNode *previous = name->GetDecl();
            cParamsNode *effectiveParams = params;
            cDeclsNode *effectiveDecls = decls;
            cStmtsNode *effectiveStmts = stmts;
            bool hasError = false;

            if (previous != nullptr)
            {
                if (!previous->IsFunc())
                {
                    SemanticParseError("Symbol " + name->GetName() +
                        " already defined in current scope", name->GetLineNumber());
                    hasError = true;
                }
                else
                {
                    cFuncDeclNode *prevFunc = static_cast<cFuncDeclNode*>(previous);

                    if (prevFunc->GetType() != type->GetDecl())
                    {
                        SemanticParseError(name->GetName() +
                            " previously declared with different return type",
                            name->GetLineNumber());
                        hasError = true;
                    }

                    if (prevFunc->NumParams() != CountParams(params))
                    {
                        SemanticParseError(name->GetName() +
                            " redeclared with a different number of parameters",
                            name->GetLineNumber());
                        hasError = true;
                    }

                    if (prevFunc->HasDefinition() && stmts != nullptr)
                    {
                        SemanticParseError(name->GetName() +
                            " already has a definition");
                        hasError = true;
                    }

                    if (effectiveParams == nullptr)
                    {
                        effectiveParams = prevFunc->GetParams();
                    }

                    if (effectiveDecls == nullptr)
                    {
                        effectiveDecls = prevFunc->GetDecls();
                    }

                    if (effectiveStmts == nullptr)
                    {
                        effectiveStmts = prevFunc->GetStmts();
                    }
                }
            }

            if (!hasError) name->SetDecl(this);
            AddChild(type);
            AddChild(name);
            if (effectiveParams != nullptr) AddChild(effectiveParams);
            if (effectiveDecls != nullptr) AddChild(effectiveDecls);
            if (effectiveStmts != nullptr) AddChild(effectiveStmts);
        }

        cDeclNode *GetType() override { return GetReturnTypeSymbol()->GetDecl(); }
        string GetName() override { return GetNameSymbol()->GetName(); }
        bool IsFunc() override { return true; }
        cSymbol *GetReturnTypeSymbol()
        {
            return static_cast<cSymbol*>(GetChild(0));
        }
        cSymbol *GetNameSymbol()
        {
            return static_cast<cSymbol*>(GetChild(1));
        }
        cParamsNode *GetParams()
        {
            if (NumChildren() > 2)
            {
                return dynamic_cast<cParamsNode*>(GetChild(2));
            }

            return nullptr;
        }
        bool HasDefinition()
        {
            return GetStmts() != nullptr;
        }
        int NumParams()
        {
            cParamsNode *params = GetParams();
            return params == nullptr ? 0 : params->NumParams();
        }
        cDeclsNode *GetDecls()
        {
            for (int i = 2; i < NumChildren(); i++)
            {
                cDeclsNode *decls = dynamic_cast<cDeclsNode*>(GetChild(i));
                if (decls != nullptr) return decls;
            }

            return nullptr;
        }
        cStmtsNode *GetStmts()
        {
            for (int i = 2; i < NumChildren(); i++)
            {
                cStmtsNode *stmts = dynamic_cast<cStmtsNode*>(GetChild(i));
                if (stmts != nullptr) return stmts;
            }

            return nullptr;
        }
        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        static int CountParams(cParamsNode *params)
        {
            return params == nullptr ? 0 : params->NumParams();
        }
};
