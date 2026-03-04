    virtual void Visit(cParamsNode *node)
    {
        // Init the offset based on function call stack overhead
        m_offset = -12;

        // Need to loop through params because computation runs
        // opposite of other vars
        for (int ii=0; ii<node->NumDecls(); ii++)
        {
            cDeclNode *param = node->GetDecl(ii);
            param->SetSize(param->GetType()->GetSize());
            param->SetOffset(m_offset);

            m_offset -= param->GetSize();
            m_offset = RoundDown(m_offset);
        }

        // Compute size based on call stack overhead
        node->SetSize(-12 - m_offset);

        // reset m_offset for funciton locals
        m_offset = 0;
    }

    int RoundDown(int value)
    {
        if (value % WORD_SIZE == 0) return value;
        return value - (WORD_SIZE + value%WORD_SIZE);
    }

