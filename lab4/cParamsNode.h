#pragma once

#include "cAstNode.h"
#include "cDeclNode.h"

class cParamsNode : public cAstNode
{
    public:
        cParamsNode(cDeclNode *param) : cAstNode()
        {
            AddChild(param);
        }

        void Insert(cDeclNode *param)
        {
            AddChild(param);
        }

        virtual string NodeType() { return string("args"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
