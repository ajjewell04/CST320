#include "cOwnedNodes.h"

std::vector<std::unique_ptr<cAstNode>> &cOwnedNodes::Nodes()
{
    static std::vector<std::unique_ptr<cAstNode>> nodes;
    return nodes;
}

void cOwnedNodes::Reset()
{
    Nodes().clear();
}
