#pragma once
//**************************************
// cProgramNode.h
//
// Defines AST node for a lang program
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cBlockNode.h"

class cProgramNode : public cAstNode
{
    public:
        // param is the block that makes up the program
        cProgramNode(cBlockNode *block) : cAstNode()
        {
            AddChild(block);
        }

        int GetSize()
        {
            return m_size;
        }
        void SetSize(int size)
        {
            m_size = size;
        }
        cBlockNode *GetBlock()
        {
            return static_cast<cBlockNode*>(GetChild(0));
        }
        virtual string NodeType() { return string("program"); }
        virtual string AttributesToString()
        {
            if (m_size == 0) return "";
            return " size=\"" + std::to_string(m_size) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_size = 0;
};
