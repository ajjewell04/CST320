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

        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
