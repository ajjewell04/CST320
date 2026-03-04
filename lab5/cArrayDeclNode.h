#pragma once

#include "cDeclNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
    public:
        cArrayDeclNode(cSymbol *type, cSymbol *name, int size) : cDeclNode()
        {
            bool hasError = (name->GetDecl() != nullptr);
            if (hasError)
            {
                SemanticParseError("Symbol " + name->GetName() +
                    " already defined in current scope", name->GetLineNumber());
            }

            m_size = size;
            if (!hasError) name->SetDecl(this);
            AddChild(type);
            AddChild(name);
        }

        cDeclNode *GetType() override
        {
            return GetElementTypeSymbol()->GetDecl();
        }
        string GetName() override { return GetNameSymbol()->GetName(); }
        bool IsArray() override { return true; }
        bool IsType() override { return true; }
        cSymbol *GetElementTypeSymbol()
        {
            return static_cast<cSymbol*>(GetChild(0));
        }
        cSymbol *GetNameSymbol()
        {
            return static_cast<cSymbol*>(GetChild(1));
        }
        int GetCount() { return m_size; }
        virtual string NodeType() { return string("array_decl"); }
        virtual string AttributesToString()
        {
            return " count=\"" + std::to_string(m_size) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_size;
};
