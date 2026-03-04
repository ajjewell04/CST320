#pragma once

#include <string>
#include "cVisitor.h"
#include <vector>

class cAssignNode;
class cArrayDeclNode;
class cBlockNode;
class cDeclsNode;
class cFuncDeclNode;
class cFuncExprNode;
class cParamListNode;
class cParamsNode;
class cProgramNode;
class cStructDeclNode;
class cVarDeclNode;
class cVarExprNode;

class cSemantics : public cVisitor
{
    public:
        cSemantics() : cVisitor() {}

        void Visit(cArrayDeclNode *node) override;
        void Visit(cAssignNode *node) override;
        void Visit(cBlockNode *node) override;
        void Visit(cDeclsNode *node) override;
        void Visit(cFuncDeclNode *node) override;
        void Visit(cFuncExprNode *node) override;
        void Visit(cParamListNode *node) override;
        void Visit(cParamsNode *node) override;
        void Visit(cProgramNode *node) override;
        void Visit(cStructDeclNode *node) override;
        void Visit(cVarDeclNode *node) override;
        void Visit(cVarExprNode *node) override;

    private:
        struct cScopeData
        {
            int baseOffset;
            int currentOffset;
            int highWater;
        };

        enum
        {
            kWordSize = 4,
            kParamBaseOffset = 12
        };

        void EnterScope(int baseOffset);
        cScopeData LeaveScope();
        cScopeData &CurrentScope();
        int AlignOffset(int offset, int size) const;
        int AllocSize(int size) const;
        int ParamSize(int size) const;
        int AllocateLocal(int size);
        cDeclNode *FindStructField(cStructDeclNode *node, const std::string &name);

        std::vector<cScopeData> m_scopes;
};
