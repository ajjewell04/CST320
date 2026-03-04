#include "cSymbolTable.h"

#include <map>
#include <memory>
#include <vector>

using std::map;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;

struct cSymbolTable::Scope
{
    map<string, shared_ptr<cSymbol>> symbols;
    Scope *parent;
    vector<unique_ptr<Scope>> children;

    explicit Scope(Scope *parentScope) : parent(parentScope) {}
};

cSymbolTable::cSymbolTable()
{
    m_rootScope.reset(new Scope(nullptr));
    m_currentScope = m_rootScope.get();
}

cSymbolTable::~cSymbolTable() = default;

symbolTable_t *cSymbolTable::IncreaseScope()
{
    m_currentScope->children.push_back(unique_ptr<Scope>(new Scope(m_currentScope)));
    m_currentScope = m_currentScope->children.back().get();

    return static_cast<symbolTable_t *>(m_currentScope);
}

symbolTable_t *cSymbolTable::DecreaseScope()
{
    if (m_currentScope != nullptr && m_currentScope->parent != nullptr)
    {
        m_currentScope = m_currentScope->parent;
    }

    return static_cast<symbolTable_t *>(m_currentScope);
}

void cSymbolTable::Insert(cSymbol *sym)
{
    if (m_currentScope == nullptr || sym == nullptr)
    {
        return;
    }

    m_currentScope->symbols[sym->GetName()] = shared_ptr<cSymbol>(sym);
}

cSymbol *cSymbolTable::Find(string name)
{
    for (Scope *scope = m_currentScope; scope != nullptr; scope = scope->parent)
    {
        auto entry = scope->symbols.find(name);
        if (entry != scope->symbols.end())
        {
            return entry->second.get();
        }
    }

    return nullptr;
}

cSymbol *cSymbolTable::FindLocal(string name)
{
    if (m_currentScope == nullptr)
    {
        return nullptr;
    }

    auto entry = m_currentScope->symbols.find(name);
    if (entry == m_currentScope->symbols.end())
    {
        return nullptr;
    }

    return entry->second.get();
}
