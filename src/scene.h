#pragma once

#include <memory>
#include <vector>

#include "prim.h"
#include "light.h"

class Scene
{
public:
    void addPrim(std::unique_ptr<Prim> p)
    {
        m_prims.emplace_back(std::move(p));
    }

    void addLight(std::unique_ptr<Light> l)
    {
        m_lights.emplace_back(std::move(l));
    }

    const std::vector<std::unique_ptr<Prim>>& getPrims() const
    {
        return m_prims;
    }

    const std::vector<std::unique_ptr<Light>>& getLights() const
    {
        return m_lights;
    }

private:

    std::vector<std::unique_ptr<Prim>> m_prims;
    std::vector<std::unique_ptr<Light>> m_lights;
};
