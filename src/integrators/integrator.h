#pragma once

#include "render/intersection.h"
#include "render/scene.h"
#include "utils/ray.h"

class Integrator
{
public:
    Integrator(const Scene& scene, uint32_t maxDepth) :  m_scene(scene), m_maxDepth(maxDepth) {}
    Vec3 intersect(Ray& ray);

protected:
    Vec3 traceRay(const Ray& ray);
    virtual Vec3 computeLo(const Ray& ray, const Intersection& inter) = 0;

    bool queryVisibility(const Ray& ray, const float tMax = FLT_MAX);
    Intersection intersectLights(const Ray& ray);

    const Scene& m_scene;
    uint32_t m_maxDepth;

private:

};
