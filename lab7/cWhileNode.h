#pragma once

#include "cStmtNode.h"
#include "cExprNode.h"

class cWhileNode : public cStmtNode
{
    public:
        cWhileNode(cExprNode *expr, cStmtNode *stmt) : cStmtNode()
        {
            AddChild(expr);
            AddChild(stmt);
        }

        cExprNode *GetExpr()
        {
            return static_cast<cExprNode*>(GetChild(0));
        }
        cStmtNode *GetStmt()
        {
            return static_cast<cStmtNode*>(GetChild(1));
        }
        virtual string NodeType() { return string("while"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
