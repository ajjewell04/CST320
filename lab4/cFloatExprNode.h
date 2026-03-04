#pragma once
//**************************************
// cFloatExprNode.h
//
// Defines an AST node for a float constant.
//

#include "cExprNode.h"

class cFloatExprNode : public cExprNode
{
    public:
        cFloatExprNode(double value) : cExprNode()
        {
            m_value = value;
        }

        virtual string AttributesToString()
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }

        virtual string NodeType() { return string("float"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        double m_value;
};
