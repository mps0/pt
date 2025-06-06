#pragma once

#include "photonmap/maxheap.h"
#include "utils/photon.h"

#include <cstdint>
#include <vector>

class KDTree
{
public:
    static constexpr size_t INVALID_INDEX = SIZE_MAX;
    struct Node
    {
        Photon photon;
        size_t depth;
        size_t left;
        size_t right;

        Node() {}

        Node(Photon& _photon, 
                size_t _depth,
                size_t _left = INVALID_INDEX,
                size_t _right = INVALID_INDEX) : 
            photon(_photon), 
            depth(_depth), 
            left(_left), 
            right(_right) {}

        bool hasChildren() const;
    };

    void createTree(std::vector<Photon>& data);
    void search(const Vec3& x, MaxHeap& heap, size_t  rootIdx, uint8_t dim) const;
    const Node& getNode(size_t idx) const;

private:

    size_t createNode(size_t start, size_t last, size_t depth, uint8_t sortDim, std::vector<Photon>& photons);

    std::vector<Node> m_tree;
};
