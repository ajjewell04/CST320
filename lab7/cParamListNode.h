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
        int GetSize()
        {
            return m_size;
        }
        void SetSize(int size)
        {
            m_size = size;
        }

        virtual string NodeType() { return string("params"); }
        virtual string AttributesToString()
        {
            if (m_size == 0) return "";
            return " size=\"" + std::to_string(m_size) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_size = 0;
};
