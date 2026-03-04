#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "cAstNode.h"

class cOwnedNodes
{
    public:
        template <typename T, typename... Args>
        static T *Make(Args&&... args)
        {
            std::unique_ptr<T> node(new T(std::forward<Args>(args)...));
            T *raw = node.get();
            Nodes().push_back(std::move(node));
            return raw;
        }

        static void Reset();

    private:
        static std::vector<std::unique_ptr<cAstNode>> &Nodes();
};
