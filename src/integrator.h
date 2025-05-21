#pragma once

#include "prim.h"
#include "ray.h"
#include "scene.h"
#include "photonmap.h"

class Integrator
{
public:
    Integrator(uint32_t maxDepth) :  m_maxDepth(maxDepth) {}
    Vec3 intersect(Ray& ray, Scene& scene, PhotonMap& photonMap, bool usePhotonMap);

private:
    Vec3 traceRay(const Ray& ray, const Scene& scene, Vec3 throughput, uint32_t depth, PhotonMap& photonMap, bool usePhotonMap, float ior = 1.0f);
    bool queryVisibility(const Ray& ray, const Scene& scene, const float tMax = FLT_MAX);
    Vec3 computeDirectLigting(const Ray& ray, const Scene& scene, const Intersection& inter);
    Intersection intersectLights(const Ray& ray, const Scene& scene);


    uint32_t m_maxDepth;
};
