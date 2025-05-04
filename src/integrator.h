#pragma once

#include "ray.h"
#include "scene.h"

class Integrator
{
public:
    Integrator(uint32_t maxDepth) :  m_maxDepth(maxDepth) {}
    Vec3 Intersect(Ray& ray, Scene& scene);

private:
    Vec3 traceRay(const Ray& ray, const Scene& scene, uint32_t depth);


    uint32_t m_maxDepth;
};
