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

        int NumParams()
        {
            return NumChildren();
        }
        cDeclNode *GetParam(int index)
        {
            return static_cast<cDeclNode*>(GetChild(index));
        }

        virtual string NodeType() { return string("args"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
