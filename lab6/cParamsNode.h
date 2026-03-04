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
        int GetSize()
        {
            return m_size;
        }
        void SetSize(int size)
        {
            m_size = size;
        }

        virtual string NodeType() { return string("args"); }
        virtual string AttributesToString()
        {
            if (m_size == 0) return "";
            return " size=\"" + std::to_string(m_size) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_size = 0;
};
