#pragma once

#include "cStmtNode.h"
#include "cExprNode.h"
#include "cStmtsNode.h"

class cIfNode : public cStmtNode
{
    public:
        cIfNode(cExprNode *expr, cStmtsNode *ifStmts, cStmtsNode *elseStmts=nullptr)
            : cStmtNode()
        {
            AddChild(expr);
            AddChild(ifStmts);
            if (elseStmts != nullptr) AddChild(elseStmts);
        }

        cExprNode *GetExpr()
        {
            return static_cast<cExprNode*>(GetChild(0));
        }
        cStmtsNode *GetIfStmts()
        {
            return static_cast<cStmtsNode*>(GetChild(1));
        }
        cStmtsNode *GetElseStmts()
        {
            return NumChildren() > 2 ? static_cast<cStmtsNode*>(GetChild(2)) : nullptr;
        }
        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
