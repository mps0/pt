#pragma once

#include "prim.h"
#include "ray.h"
#include "scene.h"

class Integrator
{
public:
    Integrator(uint32_t maxDepth) :  m_maxDepth(maxDepth) {}
    Vec3 Intersect(Ray& ray, Scene& scene);

private:
    Vec3 traceRay(const Ray& ray, const Scene& scene, Vec3 throughput, uint32_t depth);
    bool queryVisibility(const Ray& ray, const Scene& scene, const float tMax = FLT_MAX);
    Vec3 computeDirectLigting(const Ray& ray, const Scene& scene, const Intersection& inter);
    void makeHemisphereRay(const Vec3& o, const Vec3& normal, Ray& outRay, float& invPdf);


    uint32_t m_maxDepth;
};
