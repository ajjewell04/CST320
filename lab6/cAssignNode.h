#pragma once

#include "cStmtNode.h"
#include "cVarExprNode.h"
#include "cExprNode.h"

class cAssignNode : public cStmtNode
{
    public:
        cAssignNode(cVarExprNode *lval, cExprNode *expr) : cStmtNode()
        {
            AddChild(lval);
            AddChild(expr);
        }

        cVarExprNode *GetLVal()
        {
            return static_cast<cVarExprNode*>(GetChild(0));
        }
        cExprNode *GetExpr()
        {
            return static_cast<cExprNode*>(GetChild(1));
        }
        virtual string NodeType() { return string("assign"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
