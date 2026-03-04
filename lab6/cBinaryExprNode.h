#pragma once
//**************************************
// cBinaryExprNode.h
//
// Defines an AST node for binary expressions.
//

#include "cExprNode.h"
#include "cOpNode.h"
#include "cSymbolTable.h"

class cBinaryExprNode : public cExprNode
{
    public:
        cBinaryExprNode(cExprNode *left, cOpNode *op, cExprNode *right)
            : cExprNode()
        {
            AddChild(left);
            AddChild(op);
            AddChild(right);
        }

        cExprNode *GetLeft() { return static_cast<cExprNode*>(GetChild(0)); }
        cOpNode *GetOp() { return static_cast<cOpNode*>(GetChild(1)); }
        cExprNode *GetRight() { return static_cast<cExprNode*>(GetChild(2)); }
        cDeclNode *GetType() override
        {
            cDeclNode *leftType = GetLeft()->GetType();
            cDeclNode *rightType = GetRight()->GetType();
            if (leftType == nullptr || rightType == nullptr) return nullptr;

            int op = GetOp()->GetOp();
            if (op == '<' || op == '>' || op == LE || op == GE ||
                op == EQUALS || op == NOT_EQUALS || op == OR || op == AND)
            {
                cSymbol *sym = g_symbolTable.Find("int");
                return sym == nullptr ? nullptr : sym->GetDecl();
            }

            int leftRank = DeclRank(leftType);
            int rightRank = DeclRank(rightType);
            if (leftRank == 0 || rightRank == 0) return nullptr;

            return leftRank >= rightRank ? leftType : rightType;
        }
        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
