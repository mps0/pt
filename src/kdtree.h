#pragma once

#include "vec.h"
#include <cstdint>
#include <functional>
#include <iterator>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T>
class KDTree
{
private:
    static constexpr size_t INVALID_INDEX = SIZE_MAX;
    struct Node
    {
        T data;

        size_t depth;

        size_t left;
        size_t right;


        Node(T& _data, size_t _depth, size_t _left = INVALID_INDEX, size_t _right = INVALID_INDEX) :  data(_data), depth(_depth), left(_left), right(_right) {}
    };

public:

    KDTree(std::vector<T>& data)
    {
        m_tree = reinterpret_cast<Node*>(calloc(data.size(), sizeof(Node)));
        m_offset = 0; //
        createNode(0, data.size() - 1, 0, 0, data);
    }

    ~KDTree()
    {
        free(m_tree);
    }

    void printTree()
    {
        std::cout << "KD-TREE WITH " << m_offset << "ELEMENTS" << std::endl;

        printNode(0);
    }

    void printNode(size_t idx)
    {
        if(idx >= m_offset)
        {
            return;
        }
        Node& n = m_tree[idx];
        printNode(n.left);
        printNode(n.right);
    }

private:

    static bool comp(const T& a, const T& b, const uint8_t dim)
    {
        return a[dim] < b[dim];
    }

    size_t createNode(size_t start, size_t last, size_t depth, uint8_t sortDim, std::vector<T>& data)
    {
        if(start > last)
        {
            return INVALID_INDEX;
        }

        size_t dist = last - start;

        std::sort(data.begin() + start, data.begin() + last, std::bind(comp, std::placeholders::_1, std::placeholders::_2, sortDim));
        size_t medianIdx = start + dist / 2;

        size_t idx  = m_offset;
        Node& n = m_tree[idx];
        n.data = data[medianIdx];
        n.depth = depth;
        n.left = INVALID_INDEX;
        n.right = INVALID_INDEX;
        ++m_offset;
        if(dist > 1)
        {
            size_t medianIdx = start + dist / 2;
            uint8_t dim = (sortDim + 1) % 2; //TODO
            n.left = createNode(start, medianIdx, depth + 1, dim, data);
            n.right = createNode(medianIdx + 1, last, depth + 1, dim, data);
        }



        return  idx;
    }

    Node* m_tree;
    size_t m_offset;
};
