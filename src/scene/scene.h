#pragma once

#include "lights/light.h"
#include "render/prim.h"

#include <memory>
#include <vector>

class Scene
{
  public:
    void add(std::shared_ptr<Prim> p) { m_prims.emplace_back(p); }
    void add(std::shared_ptr<Light> l)
    {
        m_lights.emplace_back(l);
        if (l->getFlags() & Light::INTERSECTABLE)
        {
            auto il = reinterpret_cast<IntersectableLight*>(l.get());
            m_prims.emplace_back(il->getPrim());
        }
    }

    void add(std::shared_ptr<Scene> s)
    {
        for (auto p : s->m_prims)
        {
            add(p);
        }

        for (auto l : s->m_lights)
        {
            add(l);
        }
    }

    const std::vector<std::shared_ptr<Prim>>& getPrims() const { return m_prims; }

    const std::vector<std::shared_ptr<Light>>& getLights() const { return m_lights; }

  private:
    std::vector<std::shared_ptr<Prim>>  m_prims;
    std::vector<std::shared_ptr<Light>> m_lights;
};
