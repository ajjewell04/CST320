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

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
