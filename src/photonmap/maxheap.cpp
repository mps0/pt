#include <algorithm>
#include <cfloat>
#include <iostream>

#include "photonmap/maxheap.h"

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
            //std::cout << "SPECIAL INSERT" << std::endl;
            //print();
            m_heap.insert(iter, e);
            m_heap.pop_back();
            //std::cout << "AFATER " << std::endl;
            //print();
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

void MaxHeap::print()
{
    std::cout << "HEAP: " << std::endl;
    for(auto e : m_heap)
    {
        std::cout << "(" << e.first << ", " << e.second << ")" << std::endl;

    }
}
