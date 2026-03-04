#pragma once

#include "cStmtNode.h"
#include "cExprNode.h"

class cReturnNode : public cStmtNode
{
    public:
        cReturnNode(cExprNode *expr) : cStmtNode()
        {
            AddChild(expr);
        }

        cExprNode *GetExpr()
        {
            return static_cast<cExprNode*>(GetChild(0));
        }
        virtual string NodeType() { return string("return"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
