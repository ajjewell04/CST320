#pragma once

#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
    public:
        cArrayDeclNode(cSymbol *type, cSymbol *name, int size) : cDeclNode()
        {
            m_size = size;
            AddChild(type);
            AddChild(name);
        }

        virtual string NodeType() { return string("array_decl"); }
        virtual string AttributesToString()
        {
            return " count=\"" + std::to_string(m_size) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_size;
};
