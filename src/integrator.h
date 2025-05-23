#pragma once

#include "intersection.h"
#include "ray.h"
#include "scene.h"

class Integrator
{
public:
    Integrator(const Scene& scene, uint32_t maxDepth) :  m_scene(scene), m_maxDepth(maxDepth) {}
    Vec3 intersect(Ray& ray);

protected:
    Vec3 traceRay(const Ray& ray, Vec3 throughput, uint32_t depth, float ior = 1.0f);
    virtual Vec3 computeLo(const Ray& ray, Vec3 throughput, const Intersection& inter, uint32_t depth, uint32_t ior) = 0;

    //TODO
    bool queryVisibility(const Ray& ray, const float tMax = FLT_MAX);
    Intersection intersectLights(const Ray& ray);

    const Scene& m_scene;
    uint32_t m_maxDepth;

private:

};
