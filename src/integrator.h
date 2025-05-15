#pragma once

#include "prim.h"
#include "ray.h"
#include "scene.h"

class Integrator
{
public:
    Integrator(uint32_t maxDepth) :  m_maxDepth(maxDepth) {}
    Vec3 intersect(Ray& ray, Scene& scene);

private:
    Vec3 traceRay(const Ray& ray, const Scene& scene, Vec3 throughput, uint32_t depth, float ior = 1.0f);
    bool queryVisibility(const Ray& ray, const Scene& scene, const float tMax = FLT_MAX);
    Vec3 computeDirectLigting(const Ray& ray, const Scene& scene, const Intersection& inter);
    void makeHemisphereRay(const Vec3& o, const Vec3& normal, Ray& outRay, float& invPdf);
    Intersection intersectLights(const Ray& ray, const Scene& scene);


    uint32_t m_maxDepth;
};
