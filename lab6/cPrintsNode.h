#pragma once

#include "cStmtNode.h"

class cPrintsNode : public cStmtNode
{
    public:
        cPrintsNode(string value) : cStmtNode()
        {
            m_value = value;
        }

        virtual string NodeType() { return string("prints"); }
        virtual string AttributesToString()
        {
            return " value=\"" + m_value + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        string m_value;
};
