#pragma once
//********************************************************
// cSymbol.h - Define a class for symbols
//
// Author: Anthony Jewell
//
#include <string>

using std::string;

class cSymbol
{
    public:
        // Construct a symbol given its name
        cSymbol(string name)
        {
            m_id = ++nextId;
            m_name = name;
        }

        // Return a string representation of a symbol
        // Return value is an XML node
        string ToString()
        {
            string result("<sym id=\"");
            result += std::to_string(m_id);
            result += "\" name=\"" + m_name + "\" />";

            return result;
        }

        // Return name of symbol
        string GetName() { return m_name; }

        long long GetId() { return m_id; }
    protected:
        static long long nextId;    // keeps track of unique symbol IDs
        long long m_id;             // Unique ID for this symbol
        string m_name;              // Symbol name
};
