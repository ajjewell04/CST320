#pragma once

#include "cAstNode.h"
#include "cExprNode.h"

class cParamListNode : public cAstNode
{
    public:
        cParamListNode(cExprNode *param) : cAstNode()
        {
            AddChild(param);
        }

        void Insert(cExprNode *param)
        {
            AddChild(param);
        }

        int NumParams()
        {
            return NumChildren();
        }
        cExprNode *GetParam(int index)
        {
            return static_cast<cExprNode*>(GetChild(index));
        }

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
