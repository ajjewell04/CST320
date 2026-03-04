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
            AddChild(type);
            AddChild(name);
            if (params != nullptr) AddChild(params);
            if (decls != nullptr) AddChild(decls);
            if (stmts != nullptr) AddChild(stmts);
        }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
