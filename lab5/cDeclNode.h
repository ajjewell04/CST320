#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"

class cDeclNode;
inline int DeclRank(cDeclNode *type);

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() {}

        virtual bool IsArray()  { return false; }
        virtual bool IsStruct() { return false; }
        virtual bool IsType()   { return false; }
        virtual bool IsFunc()   { return false; }
        virtual bool IsVar()    { return false; }
        virtual bool IsFloat()  { return false; }
        virtual bool IsInt()    { return false; }
        virtual bool IsChar()   { return false; }
        virtual int GetSize()   { return 0; }
        virtual string GetName() = 0;
        virtual cDeclNode *GetType() = 0;
        bool IsCompatibleWith(cDeclNode *type)
        {
            if (type == nullptr) return false;
            if (this == type) return true;

            int thisRank = DeclRank(this);
            int typeRank = DeclRank(type);
            if (thisRank == 0 || typeRank == 0) return false;

            return typeRank <= thisRank;
        }
        virtual cDeclNode *GetDecl() override { return this; }
};

inline int DeclRank(cDeclNode *type)
{
    if (type == nullptr) return 0;
    if (type->IsChar()) return 1;
    if (type->IsInt()) return (type->GetSize() > 4) ? 3 : 2;
    if (type->IsFloat()) return (type->GetSize() > 4) ? 5 : 4;
    return 0;
}
