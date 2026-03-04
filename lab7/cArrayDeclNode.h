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

            m_count = size;
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
        bool IsVar() override { return true; }
        cSymbol *GetElementTypeSymbol()
        {
            return static_cast<cSymbol*>(GetChild(0));
        }
        cSymbol *GetNameSymbol()
        {
            return static_cast<cSymbol*>(GetChild(1));
        }
        int GetCount() { return m_count; }
        int GetSize() override
        {
            cDeclNode *elementType = GetType();
            return elementType == nullptr ? 0 : elementType->GetSize() * m_count;
        }
        virtual string NodeType() { return string("array_decl"); }
        virtual string AttributesToString()
        {
            return " count=\"" + std::to_string(m_count) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_count;
};
