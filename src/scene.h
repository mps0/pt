#pragma once

#include <vector>

#include "prim.h"
#include "light.h"

class Scene
{
public:
    void addPrim(Prim* p)
    {
        m_prims.emplace_back(p);
    }

    void addLight(Light* l)
    {
        m_lights.emplace_back(l);
        if(l->getFlags() & Light::INTERSECTABLE)
        {
            IntersectableLight* il = reinterpret_cast<IntersectableLight*>(l);
            m_prims.emplace_back(il->getPrim());
        }
    }

    const std::vector<Prim*>& getPrims() const
    {
        return m_prims;
    }

    const std::vector<Light*>& getLights() const
    {
        return m_lights;
    }

private:

    std::vector<Prim*> m_prims;
    std::vector<Light*> m_lights;
};
