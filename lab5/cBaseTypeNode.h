#pragma once
//**************************************
// cBaseTypeNode
//
// Defines virtual base class for all declarations.
//
// Author: Phil Howard 
//

#include "cDeclNode.h"
#include "cSymbolTable.h"

class cBaseTypeNode : public cDeclNode
{
    public:
        cBaseTypeNode(string name, int size, bool isFloat)
            : cDeclNode() 
        {
            m_name = name;
            m_size = size;
            m_isFloat = isFloat;
        }

        // return various Is* values
        virtual bool IsFloat() { return m_isFloat; }
        virtual bool IsInt()   { return !m_isFloat; }
        virtual bool IsChar()  { return (!m_isFloat && m_size==1); }
        virtual bool IsType()  { return true; }

        // return the symbol for the type
        virtual cDeclNode *GetType() { return this; }

        virtual string GetName() { return m_name; }

        virtual string NodeType() { return "type"; }
        virtual string AttributesToString()
        {
            return " name=\"" + m_name + "\" size=\"" + 
                std::to_string(m_size) +
                "\" isFloat=\"" + std::to_string(m_isFloat) + "\"";
        }

        // return size of data item
        virtual int GetSize() { return m_size; }

        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        string m_name;
        int    m_size;
        bool   m_isFloat;
};
