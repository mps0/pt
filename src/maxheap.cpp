#include <algorithm>
#include <cfloat>

#include "maxheap.h"

bool MaxHeap::insert(IdxDist e)
{
    //have to sort!
    auto comp = [](const IdxDist& a, const IdxDist& b) -> bool
    {
        return a.second < b.second;
    };

    auto iter = std::upper_bound(m_heap.begin(), m_heap.end(), e, comp);

    if(m_heap.size() < m_size)
    {
        m_heap.insert(iter, e);
        return true;
    }
    else
    {
        if(iter != m_heap.end())
        {
            m_heap.insert(iter, e);
            m_heap.pop_back();
            return true;
        }
    }

    return false;
}

float MaxHeap::getMaxDist() const
{
    if(m_heap.size() < m_size)
    {
        return FLT_MAX;
    }

    return m_heap.back().second;
}

const std::vector<IdxDist>& MaxHeap::getHeap()
{
    return m_heap;
}
