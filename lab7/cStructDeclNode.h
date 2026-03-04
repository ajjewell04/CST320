#pragma once

#include "cDeclNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"

class cStructDeclNode : public cDeclNode
{
    public:
        cStructDeclNode(cDeclsNode *decls, cSymbol *name) : cDeclNode()
        {
            bool hasError = (name->GetDecl() != nullptr);
            if (hasError)
            {
                SemanticParseError("Symbol " + name->GetName() +
                    " already defined in current scope", name->GetLineNumber());
            }

            if (!hasError) name->SetDecl(this);
            AddChild(decls);
            AddChild(name);
        }

        cDeclNode *GetType() override { return this; }
        string GetName() override { return GetNameSymbol()->GetName(); }
        bool IsStruct() override { return true; }
        bool IsType() override { return true; }
        cDeclsNode *GetFields()
        {
            return static_cast<cDeclsNode*>(GetChild(0));
        }
        cSymbol *GetNameSymbol()
        {
            return static_cast<cSymbol*>(GetChild(1));
        }
        virtual string NodeType() { return string("struct_decl"); }
        virtual string AttributesToString()
        {
            if (GetSize() == 0 && GetOffset() == 0) return "";

            return " size=\"" + std::to_string(GetSize()) +
                "\" offset=\"" + std::to_string(GetOffset()) + "\"";
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
