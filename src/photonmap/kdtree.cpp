#include "photonmap/kdtree.h"

#include <functional>
#include <algorithm>

bool KDTree::Node::hasChildren() const
{
    return left != INVALID_INDEX || right != INVALID_INDEX;
}

void KDTree::createTree(std::vector<Photon>& data)
{
    if(data.size() > 0)
    {
        std::cout << "CREATING KD TREE, num photons: " << data.size() << std::endl;
        m_tree.reserve(data.size());
        createNode(0, data.size() - 1, 0, 0, data);
    }
}

void KDTree::search(const Vec3& x, MaxHeap& heap, size_t  rootIdx, uint8_t dim) const
{
    if(rootIdx == INVALID_INDEX)
    {
        return;
    }

    // has children
    const Node& n = m_tree[rootIdx];
    const Photon& p = n.photon;

    Vec3 pToX = x - p.wPos;

    if(n.hasChildren())
    {
        float distToSplit = pToX[dim];
        float distToSplitSquared = distToSplit * distToSplit;

        // left side of split
        if(distToSplit < 0.0f)
        {
            search(x, heap, m_tree[rootIdx].left, (dim + 1) % 3);
            // prune right if possible
            if(distToSplitSquared < heap.getMaxDist())
            {
                search(x, heap, m_tree[rootIdx].right, (dim + 1) % 3);
            }
        }
        else
        {
            search(x, heap, m_tree[rootIdx].right, (dim + 1) % 3);
            // prune left if possible
            if(distToSplitSquared < heap.getMaxDist())
            {
                search(x, heap, m_tree[rootIdx].left, (dim + 1) % 3);
            }
        }
    }

    float distToPhotonSquared = dot(pToX, pToX);
    heap.insert({rootIdx, distToPhotonSquared});
}

const KDTree::Node& KDTree::getNode(size_t idx) const
{
    return m_tree[idx];
}

size_t KDTree::createNode(size_t start, size_t last, size_t depth, uint8_t sortDim, std::vector<Photon>& photons)
{
    if(start > last)
    {
        return INVALID_INDEX;
    }

    size_t dist = last - start;

    auto comp = [](const Photon& a, const Photon& b, const uint8_t dim) -> bool
    {
        return a.wPos[dim] < b.wPos[dim];
    };

    std::sort(photons.begin() + start, photons.begin() + last, std::bind(comp, std::placeholders::_1, std::placeholders::_2, sortDim));
    size_t medianIdx = start + dist / 2;

    size_t idx  = m_tree.size();
    m_tree.emplace_back();
    Node& n = m_tree.back();
    n.photon = photons[medianIdx];
    n.depth = depth;
    n.left = INVALID_INDEX;
    n.right = INVALID_INDEX;
    if(dist > 0)
    {
        size_t medianIdx = start + dist / 2;
        uint8_t dim = (sortDim + 1) % 3;
        if(medianIdx > 0)
        {
            n.left = createNode(start, medianIdx - 1, depth + 1, dim, photons);
        }
        n.right = createNode(medianIdx + 1, last, depth + 1, dim, photons);
    }

    return  idx;
}
