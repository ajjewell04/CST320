#pragma once
//**************************************
// cVarExprNode.h
//
// Defines an AST node for variable references.
//

#include "cExprNode.h"
#include "cStructDeclNode.h"
#include "cSymbol.h"
#include <vector>

class cVarExprNode : public cExprNode
{
    public:
        cVarExprNode(cSymbol *name) : cExprNode(), m_size(0), m_offset(0)
        {
            if (name->GetDecl() == nullptr)
            {
                SemanticParseError("Symbol " + name->GetName() + " not defined",
                    name->GetLineNumber());
            }

            AddChild(name);
        }

        void AddPart(cAstNode *part)
        {
            AddChild(part);
        }

        cDeclNode *GetType() override
        {
            cDeclNode *decl = GetDecl();
            if (decl == nullptr) return nullptr;

            cDeclNode *type = decl->IsArray() ? decl : decl->GetType();
            for (int i = 1; i < NumChildren(); i++)
            {
                cExprNode *index = dynamic_cast<cExprNode*>(GetChild(i));
                if (index != nullptr)
                {
                    if (type != nullptr && type->IsArray())
                    {
                        type = type->GetType();
                    }
                    else
                    {
                        return nullptr;
                    }
                }
                else
                {
                    cSymbol *fieldSym = dynamic_cast<cSymbol*>(GetChild(i));
                    if (fieldSym == nullptr || type == nullptr || !type->IsStruct())
                    {
                        return nullptr;
                    }

                    cStructDeclNode *structType = static_cast<cStructDeclNode*>(type);
                    cDeclsNode *fields = structType->GetFields();
                    cDeclNode *fieldDecl = nullptr;

                    for (int field = 0; fields != nullptr && field < fields->NumDecls(); field++)
                    {
                        cDeclNode *candidate = fields->GetDecl(field);
                        if (candidate != nullptr && candidate->GetName() == fieldSym->GetName())
                        {
                            fieldDecl = candidate;
                            break;
                        }
                    }

                    if (fieldDecl == nullptr)
                    {
                        return nullptr;
                    }

                    type = fieldDecl->GetType();
                }
            }

            return type;
        }
        cDeclNode *GetDecl() override
        {
            return GetNameSymbol()->GetDecl();
        }
        cSymbol *GetNameSymbol()
        {
            return static_cast<cSymbol*>(GetChild(0));
        }
        int GetPartCount()
        {
            return NumChildren() - 1;
        }
        cAstNode *GetPart(int index)
        {
            return GetChild(index + 1);
        }
        virtual string NodeType() { return string("varref"); }
        int GetSize()
        {
            return m_size;
        }
        void SetSize(int size)
        {
            m_size = size;
        }
        int GetOffset()
        {
            return m_offset;
        }
        void SetOffset(int offset)
        {
            m_offset = offset;
        }
        std::vector<int> GetRowSizes()
        {
            return m_rowSizes;
        }
        void SetRowSizes(std::vector<int> rowSizes)
        {
            m_rowSizes = rowSizes;
        }
        virtual string AttributesToString()
        {
            if (m_size == 0 && m_offset == 0 && m_rowSizes.empty()) return "";

            string result = " size=\"" + std::to_string(m_size) +
                "\" offset=\"" + std::to_string(m_offset) + "\"";

            if (!m_rowSizes.empty())
            {
                result += " rowsizes=\"";
                for (size_t i = 0; i < m_rowSizes.size(); i++)
                {
                    if (i != 0) result += " ";
                    result += std::to_string(m_rowSizes[i]);
                }
                result += "\"";
            }

            return result;
        }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    protected:
        int m_size;
        int m_offset;
        std::vector<int> m_rowSizes;
};
