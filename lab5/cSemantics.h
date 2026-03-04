#pragma once

#include "cVisitor.h"

class cAssignNode;
class cFuncExprNode;
class cVarExprNode;

class cSemantics : public cVisitor
{
    public:
        cSemantics() : cVisitor() {}

        void Visit(cAssignNode *node) override;
        void Visit(cFuncExprNode *node) override;
        void Visit(cVarExprNode *node) override;
};
