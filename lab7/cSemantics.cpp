#include "cSemantics.h"

#include <algorithm>

#include "astnodes.h"

void cSemantics::EnterScope(int baseOffset)
{
    m_scopes.push_back({baseOffset, baseOffset, baseOffset});
}

cSemantics::cScopeData cSemantics::LeaveScope()
{
    cScopeData scope = m_scopes.back();
    m_scopes.pop_back();
    return scope;
}

cSemantics::cScopeData &cSemantics::CurrentScope()
{
    return m_scopes.back();
}

int cSemantics::AlignOffset(int offset, int size) const
{
    if (size <= 1) return offset;

    int remainder = offset % kWordSize;
    if (remainder == 0) return offset;
    return offset + (kWordSize - remainder);
}

int cSemantics::AllocSize(int size) const
{
    return size < 0 ? 0 : size;
}

int cSemantics::ParamSize(int size) const
{
    int allocSize = AlignOffset(AllocSize(size), size);
    return std::max(static_cast<int>(kWordSize), allocSize);
}

int cSemantics::FrameSize(int size) const
{
    return AlignOffset(AllocSize(size), kWordSize);
}

int cSemantics::AllocateLocal(int size)
{
    cScopeData &scope = CurrentScope();
    int allocSize = AllocSize(size);
    int offset = AlignOffset(scope.currentOffset, allocSize);
    scope.currentOffset = offset + allocSize;
    scope.highWater = std::max(scope.highWater, scope.currentOffset);
    return offset;
}

cDeclNode *cSemantics::FindStructField(cStructDeclNode *node, const std::string &name)
{
    if (node == nullptr) return nullptr;

    cDeclsNode *fields = node->GetFields();
    for (int i = 0; fields != nullptr && i < fields->NumDecls(); i++)
    {
        cDeclNode *field = fields->GetDecl(i);
        if (field != nullptr && field->GetName() == name)
        {
            return field;
        }
    }

    return nullptr;
}

void cSemantics::Visit(cArrayDeclNode *node)
{
    node->VisitAllChildren(this);

    int size = node->GetSize();
    node->SetSize(size);
    node->SetOffset(AllocateLocal(size));
}

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

void cSemantics::Visit(cBlockNode *node)
{
    int baseOffset = m_scopes.empty() ? 0 : CurrentScope().currentOffset;

    EnterScope(baseOffset);
    node->VisitAllChildren(this);

    cScopeData scope = LeaveScope();
    node->SetSize(scope.highWater - scope.baseOffset);

    if (!m_scopes.empty())
    {
        CurrentScope().highWater = std::max(CurrentScope().highWater, scope.highWater);
    }
}

void cSemantics::Visit(cDeclsNode *node)
{
    node->VisitAllChildren(this);

    if (!m_scopes.empty())
    {
        cScopeData &scope = CurrentScope();
        node->SetSize(scope.currentOffset - scope.baseOffset);
    }
}

void cSemantics::Visit(cFuncDeclNode *node)
{
    cParamsNode *params = node->GetParams();
    cDeclsNode *decls = node->GetDecls();
    cStmtsNode *stmts = node->GetStmts();

    if (params != nullptr)
    {
        params->Visit(this);
    }

    if (decls != nullptr || stmts != nullptr)
    {
        EnterScope(0);

        if (decls != nullptr) decls->Visit(this);
        if (stmts != nullptr) stmts->Visit(this);

        cScopeData scope = LeaveScope();
        int size = scope.highWater - scope.baseOffset;
        node->SetSize(FrameSize(size));
    }
    else
    {
        node->SetSize(0);
    }

    node->SetOffset(0);
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

void cSemantics::Visit(cParamListNode *node)
{
    node->VisitAllChildren(this);

    int size = 0;
    for (int i = 0; i < node->NumParams(); i++)
    {
        cExprNode *param = node->GetParam(i);
        cDeclNode *type = param == nullptr ? nullptr : param->GetType();
        size += ParamSize(type == nullptr ? 0 : type->GetSize());
    }

    node->SetSize(size);
}

void cSemantics::Visit(cParamsNode *node)
{
    int size = 0;

    for (int i = 0; i < node->NumParams(); i++)
    {
        cDeclNode *param = node->GetParam(i);
        cDeclNode *type = param == nullptr ? nullptr : param->GetType();
        int paramSize = type == nullptr ? 0 : type->GetSize();
        int slotSize = ParamSize(paramSize);

        if (param != nullptr)
        {
            param->SetSize(paramSize);
            param->SetOffset(-(kParamBaseOffset + size));
        }

        size += slotSize;
    }

    node->SetSize(size);
}

void cSemantics::Visit(cProgramNode *node)
{
    node->VisitAllChildren(this);

    cBlockNode *block = node->GetBlock();
    int size = block == nullptr ? 0 : block->GetSize();
    node->SetSize(FrameSize(size));
}

void cSemantics::Visit(cStructDeclNode *node)
{
    EnterScope(0);

    cDeclsNode *fields = node->GetFields();
    if (fields != nullptr) fields->Visit(this);

    cScopeData scope = LeaveScope();
    node->SetSize(scope.currentOffset - scope.baseOffset);
    node->SetOffset(0);
}

void cSemantics::Visit(cVarDeclNode *node)
{
    node->VisitAllChildren(this);

    cDeclNode *type = node->GetType();
    int size = type == nullptr ? 0 : type->GetSize();

    node->SetSize(size);
    node->SetOffset(AllocateLocal(size));
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

    cDeclNode *type = decl->IsArray() ? decl : decl->GetType();
    string name = node->GetNameSymbol()->GetName();
    int offset = decl->GetOffset();
    int size = decl->GetSize();
    std::vector<int> rowSizes;

    for (int i = 0; i < node->GetPartCount(); i++)
    {
        cExprNode *index = dynamic_cast<cExprNode*>(node->GetPart(i));
        if (index != nullptr)
        {
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

            cDeclNode *elementType = type->GetType();
            if (elementType != nullptr)
            {
                rowSizes.push_back(elementType->GetSize());
                type = elementType;
            }
            else
            {
                type = nullptr;
            }

            continue;
        }

        cSymbol *fieldSym = dynamic_cast<cSymbol*>(node->GetPart(i));
        if (fieldSym == nullptr)
        {
            continue;
        }

        if (type == nullptr || !type->IsStruct())
        {
            node->SemanticError(name + " is not a struct");
            return;
        }

        cDeclNode *field = FindStructField(static_cast<cStructDeclNode*>(type),
            fieldSym->GetName());
        if (field == nullptr)
        {
            node->SemanticError(fieldSym->GetName() + " is not a field of " + name);
            return;
        }

        offset += field->GetOffset();
        size = field->GetSize();
        type = field->GetType();
        name += "." + fieldSym->GetName();
    }

    node->SetSize(size);
    node->SetOffset(offset);
    node->SetRowSizes(rowSizes);
}
