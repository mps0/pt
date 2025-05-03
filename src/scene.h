#pragma once

#include <memory>
#include <vector>

#include "prim.h"

class Scene
{
public:
    void add(std::unique_ptr<Prim> p)
    {
        m_prims.emplace_back(std::move(p));
    }

    const std::vector<std::unique_ptr<Prim>>& getPrims() const
    {
        return m_prims;
    }

private:

    std::vector<std::unique_ptr<Prim>> m_prims;
};
