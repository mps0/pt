#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

using IdxDist = std::pair<size_t, float>;

class MaxHeap
{
public:
    MaxHeap(size_t size) : m_size(size) {}

    bool insert(IdxDist e);
    float getMaxDist() const;
    const std::vector<IdxDist>& getHeap();
    std::string print();

private:

    size_t m_size;
    std::vector<IdxDist> m_heap;
};
