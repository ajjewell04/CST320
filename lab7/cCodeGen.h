#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "cVisitor.h"

class cAstNode;
class cAssignNode;
class cBinaryExprNode;
class cBlockNode;
class cFuncDeclNode;
class cFuncExprNode;
class cIfNode;
class cIntExprNode;
class cParamListNode;
class cPrintNode;
class cPrintsNode;
class cProgramNode;
class cReturnNode;
class cStmtNode;
class cStmtsNode;
class cVarExprNode;
class cWhileNode;
class cFloatExprNode;

class cCodeGen : public cVisitor
{
    public:
        cCodeGen() : cVisitor() {}

        void Generate(cProgramNode *node);
        void RegisterFunction(cFuncDeclNode *node);

        void Visit(cAssignNode *node) override;
        void Visit(cBinaryExprNode *node) override;
        void Visit(cBlockNode *node) override;
        void Visit(cFloatExprNode *node) override;
        void Visit(cFuncDeclNode *node) override;
        void Visit(cFuncExprNode *node) override;
        void Visit(cIfNode *node) override;
        void Visit(cIntExprNode *node) override;
        void Visit(cParamListNode *node) override;
        void Visit(cPrintNode *node) override;
        void Visit(cPrintsNode *node) override;
        void Visit(cProgramNode *node) override;
        void Visit(cReturnNode *node) override;
        void Visit(cStmtsNode *node) override;
        void Visit(cVarExprNode *node) override;
        void Visit(cWhileNode *node) override;

    private:
        void CollectFunctions(cAstNode *node);
        void EmitFunction(cFuncDeclNode *node);
        void EmitStatement(cStmtNode *node);
        void EmitAddress(cVarExprNode *node);
        void EmitLoadFromAddress(int size);
        void EmitStoreToAddress(int size);
        void EmitPushInt(int value);
        void EmitLine(const std::string &text);
        std::string EscapeString(const std::string &value) const;
        std::string GetFunctionLabel(cFuncDeclNode *node);
        std::string NextFunctionLabel();

        std::unordered_map<cFuncDeclNode*, std::string> m_funcLabels;
        std::vector<cFuncDeclNode*> m_functions;
        cFuncDeclNode *m_currentFunction = nullptr;
        int m_nextFunctionId = 0;
};
