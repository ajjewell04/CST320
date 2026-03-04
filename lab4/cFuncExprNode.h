#pragma once

#include "cExprNode.h"
#include "cParamListNode.h"
#include "cSymbol.h"

class cFuncExprNode : public cExprNode
{
    public:
        cFuncExprNode(cSymbol *name, cParamListNode *params=nullptr) : cExprNode()
        {
            AddChild(name);
            if (params != nullptr) AddChild(params);
        }

        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
