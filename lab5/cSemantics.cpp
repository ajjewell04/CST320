#include "cSemantics.h"

#include "astnodes.h"

void cSemantics::Visit(cAssignNode *node)
{
    node->VisitAllChildren(this);

    if (node->GetLVal()->HasSemanticError() || node->GetExpr()->HasSemanticError())
    {
        return;
    }

    cDeclNode *leftType = node->GetLVal()->GetType();
    cDeclNode *rightType = node->GetExpr()->GetType();
    if (leftType == nullptr || rightType == nullptr)
    {
        return;
    }

    if (!leftType->IsCompatibleWith(rightType))
    {
        node->SemanticError("Cannot assign " + rightType->GetName() +
            " to " + leftType->GetName());
    }
}

void cSemantics::Visit(cFuncExprNode *node)
{
    node->VisitAllChildren(this);

    cDeclNode *decl = node->GetDecl();
    if (decl == nullptr || !decl->IsFunc())
    {
        return;
    }

    cFuncDeclNode *funcDecl = static_cast<cFuncDeclNode*>(decl);
    cParamListNode *actuals = node->GetParams();
    int actualCount = actuals == nullptr ? 0 : actuals->NumParams();
    if (actualCount != funcDecl->NumParams())
    {
        node->SemanticError(funcDecl->GetName() +
            " called with wrong number of arguments");
    }
    else if (actuals != nullptr)
    {
        cParamsNode *formals = funcDecl->GetParams();
        for (int i = 0; i < actualCount; i++)
        {
            cExprNode *actual = actuals->GetParam(i);
            if (actual == nullptr || actual->HasSemanticError())
            {
                continue;
            }

            cDeclNode *actualType = actual->GetType();
            cDeclNode *formalType = formals->GetParam(i)->GetType();
            if (actualType == nullptr || formalType == nullptr)
            {
                continue;
            }

            if (!formalType->IsCompatibleWith(actualType))
            {
                node->SemanticError(funcDecl->GetName() +
                    " called with incompatible argument");
                break;
            }
        }
    }

    if (!funcDecl->HasDefinition())
    {
        node->SemanticError("Function " + funcDecl->GetName() +
            " is not fully defined");
    }
}

void cSemantics::Visit(cVarExprNode *node)
{
    node->VisitAllChildren(this);

    cDeclNode *decl = node->GetDecl();
    if (decl == nullptr)
    {
        return;
    }

    if (decl->IsFunc())
    {
        node->SemanticError("Symbol " + node->GetNameSymbol()->GetName() +
            " is a function, not a variable");
        return;
    }

    cDeclNode *type = decl->GetType();
    string name = node->GetNameSymbol()->GetName();
    for (int i = 0; i < node->GetPartCount(); i++)
    {
        cExprNode *index = dynamic_cast<cExprNode*>(node->GetPart(i));
        if (index == nullptr)
        {
            continue;
        }

        if (type == nullptr || !type->IsArray())
        {
            node->SemanticError(name + " is not an array");
            return;
        }

        cDeclNode *indexType = index->GetType();
        if (indexType != nullptr && !indexType->IsInt())
        {
            node->SemanticError("Index of " + name + " is not an int");
            return;
        }

        type = type->GetType();
    }
}
