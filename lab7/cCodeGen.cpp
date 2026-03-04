#include "cCodeGen.h"

#include <string>

#include "astnodes.h"
#include "emit.h"
#include "langparse.h"

namespace
{
    class cFunctionCollector : public cVisitor
    {
        public:
            explicit cFunctionCollector(cCodeGen *codegen) : m_codegen(codegen) {}

            void Visit(cFuncDeclNode *node) override
            {
                if (node->HasDefinition())
                {
                    m_codegen->RegisterFunction(node);
                }

                node->VisitAllChildren(this);
            }

        private:
            cCodeGen *m_codegen;
    };
}

void cCodeGen::Generate(cProgramNode *node)
{
    if (node != nullptr)
    {
        Visit(node);
    }
}

void cCodeGen::RegisterFunction(cFuncDeclNode *node)
{
    GetFunctionLabel(node);
    m_functions.push_back(node);
}

void cCodeGen::CollectFunctions(cAstNode *node)
{
    cFunctionCollector collector(this);
    collector.VisitAllNodes(node);
}

std::string cCodeGen::NextFunctionLabel()
{
    m_nextFunctionId++;
    return "lang_func_" + std::to_string(m_nextFunctionId);
}

std::string cCodeGen::GetFunctionLabel(cFuncDeclNode *node)
{
    auto it = m_funcLabels.find(node);
    if (it != m_funcLabels.end())
    {
        return it->second;
    }

    std::string label = NextFunctionLabel();
    m_funcLabels[node] = label;
    return label;
}

void cCodeGen::EmitLine(const std::string &text)
{
    EmitString(text);
    EmitString("\n");
}

void cCodeGen::EmitPushInt(int value)
{
    EmitString("PUSH ");
    EmitInt(value);
    EmitString("\n");
}

std::string cCodeGen::EscapeString(const std::string &value) const
{
    std::string result;

    for (char ch : value)
    {
        if (ch == '\\' || ch == '"')
        {
            result += '\\';
        }

        result += ch;
    }

    return result;
}

void cCodeGen::EmitLoadFromAddress(int size)
{
    if (size == 1)
    {
        EmitLine("PUSHCVARIND");
    }
    else
    {
        EmitLine("PUSHVARIND");
    }
}

void cCodeGen::EmitStoreToAddress(int size)
{
    if (size == 1)
    {
        EmitLine("POPCVARIND");
    }
    else
    {
        EmitLine("POPVARIND");
    }
}

void cCodeGen::EmitAddress(cVarExprNode *node)
{
    EmitPushInt(node->GetOffset());
    EmitLine("PUSHFP");
    EmitLine("PLUS");

    int rowIndex = 0;
    std::vector<int> rowSizes = node->GetRowSizes();
    for (int i = 0; i < node->GetPartCount(); i++)
    {
        cExprNode *index = dynamic_cast<cExprNode*>(node->GetPart(i));
        if (index == nullptr)
        {
            continue;
        }

        index->Visit(this);
        EmitPushInt(rowSizes[rowIndex++]);
        EmitLine("TIMES");
        EmitLine("PLUS");
    }
}

void cCodeGen::EmitStatement(cStmtNode *node)
{
    if (node == nullptr)
    {
        return;
    }

    node->Visit(this);
    if (dynamic_cast<cFuncExprNode*>(node) != nullptr)
    {
        EmitLine("POP");
    }
}

void cCodeGen::EmitFunction(cFuncDeclNode *node)
{
    std::string label = GetFunctionLabel(node);

    EmitString(".function ");
    EmitString(label);
    EmitString("\n");
    EmitString(label);
    EmitString(":\n");

    if (node->GetSize() > 0)
    {
        EmitString("ADJSP ");
        EmitInt(node->GetSize());
        EmitString("\n");
    }

    cFuncDeclNode *previous = m_currentFunction;
    m_currentFunction = node;

    cStmtsNode *stmts = node->GetStmts();
    if (stmts != nullptr)
    {
        stmts->Visit(this);
    }

    EmitPushInt(0);
    EmitLine("RETURNV");

    m_currentFunction = previous;
}

void cCodeGen::Visit(cProgramNode *node)
{
    m_functions.clear();
    m_funcLabels.clear();
    m_nextFunctionId = 0;

    CollectFunctions(node);

    EmitLine(".function main");
    EmitLine("main:");
    if (node->GetSize() > 0)
    {
        EmitString("ADJSP ");
        EmitInt(node->GetSize());
        EmitString("\n");
    }

    cBlockNode *block = node->GetBlock();
    if (block != nullptr)
    {
        block->Visit(this);
    }

    EmitPushInt(0);
    EmitLine("RETURNV");

    for (cFuncDeclNode *func : m_functions)
    {
        EmitFunction(func);
    }
}

void cCodeGen::Visit(cFuncDeclNode *node)
{
    if (node->HasDefinition())
    {
        GetFunctionLabel(node);
    }
}

void cCodeGen::Visit(cBlockNode *node)
{
    cStmtsNode *stmts = node->GetStmts();
    if (stmts != nullptr)
    {
        stmts->Visit(this);
    }
}

void cCodeGen::Visit(cStmtsNode *node)
{
    for (int i = 0; i < node->NumStmts(); i++)
    {
        EmitStatement(node->GetStmt(i));
    }
}

void cCodeGen::Visit(cIntExprNode *node)
{
    EmitPushInt(node->GetValue());
}

void cCodeGen::Visit(cFloatExprNode *node)
{
    EmitPushInt(static_cast<int>(node->GetValue()));
}

void cCodeGen::Visit(cVarExprNode *node)
{
    EmitAddress(node);
    EmitLoadFromAddress(node->GetSize());
}

void cCodeGen::Visit(cAssignNode *node)
{
    node->GetExpr()->Visit(this);
    EmitAddress(node->GetLVal());
    EmitStoreToAddress(node->GetLVal()->GetSize());
}

void cCodeGen::Visit(cBinaryExprNode *node)
{
    int op = node->GetOp()->GetOp();

    if (op == OR)
    {
        std::string evalRight = GenerateLabel();
        std::string end = GenerateLabel();

        node->GetLeft()->Visit(this);
        EmitLine("DUP");
        EmitString("JUMPE @");
        EmitString(evalRight);
        EmitString("\n");
        EmitLine("POP");
        EmitPushInt(1);
        EmitString("JUMP @");
        EmitString(end);
        EmitString("\n");
        EmitString(evalRight);
        EmitString(":\n");
        EmitLine("POP");
        node->GetRight()->Visit(this);
        EmitPushInt(0);
        EmitLine("NE");
        EmitString(end);
        EmitString(":\n");
        return;
    }

    if (op == AND)
    {
        std::string falseLabel = GenerateLabel();
        std::string end = GenerateLabel();

        node->GetLeft()->Visit(this);
        EmitLine("DUP");
        EmitString("JUMPE @");
        EmitString(falseLabel);
        EmitString("\n");
        EmitLine("POP");
        node->GetRight()->Visit(this);
        EmitPushInt(0);
        EmitLine("NE");
        EmitString("JUMP @");
        EmitString(end);
        EmitString("\n");
        EmitString(falseLabel);
        EmitString(":\n");
        EmitLine("POP");
        EmitPushInt(0);
        EmitString(end);
        EmitString(":\n");
        return;
    }

    node->GetLeft()->Visit(this);
    node->GetRight()->Visit(this);

    switch (op)
    {
        case '+': EmitLine("PLUS"); break;
        case '-': EmitLine("MINUS"); break;
        case '*': EmitLine("TIMES"); break;
        case '/': EmitLine("DIVIDE"); break;
        case '%': EmitLine("MOD"); break;
        case '<': EmitLine("LT"); break;
        case '>': EmitLine("GT"); break;
        case LE: EmitLine("LE"); break;
        case GE: EmitLine("GE"); break;
        case EQUALS: EmitLine("EQ"); break;
        case NOT_EQUALS: EmitLine("NE"); break;
    }
}

void cCodeGen::Visit(cFuncExprNode *node)
{
    cParamListNode *params = node->GetParams();
    if (params != nullptr)
    {
        params->Visit(this);
    }

    EmitString("CALL @");
    EmitString(GetFunctionLabel(static_cast<cFuncDeclNode*>(node->GetDecl())));
    EmitString("\n");
}

void cCodeGen::Visit(cParamListNode *node)
{
    for (int i = node->NumParams() - 1; i >= 0; i--)
    {
        node->GetParam(i)->Visit(this);
    }
}

void cCodeGen::Visit(cPrintNode *node)
{
    node->GetExpr()->Visit(this);
    EmitLine("CALL @print");
    EmitLine("POP");
}

void cCodeGen::Visit(cPrintsNode *node)
{
    std::string label = GenerateLabel();

    EmitString("PUSH @");
    EmitString(label);
    EmitString("\n");
    EmitLine("OUTS");
    EmitLine(".dataseg");
    EmitString(label);
    EmitString(":\n");
    EmitString(".string \"");
    EmitString(EscapeString(node->GetValue()));
    EmitString("\"\n");
    EmitLine(".codeseg");
}

void cCodeGen::Visit(cReturnNode *node)
{
    node->GetExpr()->Visit(this);
    EmitLine("RETURNV");
}

void cCodeGen::Visit(cIfNode *node)
{
    std::string elseLabel = GenerateLabel();
    std::string endLabel = GenerateLabel();

    node->GetExpr()->Visit(this);
    EmitString("JUMPE @");
    EmitString(elseLabel);
    EmitString("\n");

    cStmtsNode *ifStmts = node->GetIfStmts();
    if (ifStmts != nullptr)
    {
        ifStmts->Visit(this);
    }

    EmitString("JUMP @");
    EmitString(endLabel);
    EmitString("\n");
    EmitString(elseLabel);
    EmitString(":\n");

    cStmtsNode *elseStmts = node->GetElseStmts();
    if (elseStmts != nullptr)
    {
        elseStmts->Visit(this);
    }

    EmitString(endLabel);
    EmitString(":\n");
}

void cCodeGen::Visit(cWhileNode *node)
{
    std::string startLabel = GenerateLabel();
    std::string endLabel = GenerateLabel();

    EmitString(startLabel);
    EmitString(":\n");
    node->GetExpr()->Visit(this);
    EmitString("JUMPE @");
    EmitString(endLabel);
    EmitString("\n");
    EmitStatement(node->GetStmt());
    EmitString("JUMP @");
    EmitString(startLabel);
    EmitString("\n");
    EmitString(endLabel);
    EmitString(":\n");
}
