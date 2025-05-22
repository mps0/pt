#include <cmath>

#include "integrator.h"
#include "defs.h"
#include "material.h"
#include "prim.h"
#include "ray.h"
#include "sampler.h"
#include "vec.h"
#include "utils.h"

Vec3 Integrator::intersect(Ray& ray)
{
    return traceRay(ray, Vec3(1.f), 0);
}

Vec3 Integrator::traceRay(const Ray& ray, Vec3 throughput, uint32_t depth, float ior)
{
    Vec3 Lo = Vec3(0.0f);
    //if(depth > 0)
    if(depth > m_maxDepth)
    {
        return Lo;
    }

    std::vector<Prim*> prims = m_scene.getPrims();
    Intersection rInter{false, FLT_MAX, Vec3(), Vec3(), nullptr};
    for(Prim* p : prims)
    {
        Intersection inter = p->Intersect(ray);
        if(inter.hit && inter.t < rInter.t)
        {
            rInter = inter;
        }
    }

    if(!rInter.hit)
    {
        return Lo;
    }

    Lo = computeLo(ray, throughput, rInter, depth, ior);

    Ray outRay;
    float outRayInvPdf;
    makeHemisphereRay(rInter.hitPoint, rInter.normal, outRay, outRayInvPdf);

    Vec3 brdf = rInter.mat->evalBrdf(outRay.d, ray.d, rInter.hitPoint);

    Vec3 temp = brdf * throughput * dot(rInter.normal, outRay.d) * outRayInvPdf;
    throughput = throughput * temp;

    float zeta = Sampler::the().sampleUniformUnitInterval();
    float p = std::max(std::min(1.0f, max(throughput)), 0.001f);

    if(zeta < p)
    {
        //bounce
        Lo = Lo + (temp * traceRay(outRay, throughput, depth + 1, ior)) * (1.0f / p);
    }
    return Lo;
}

bool Integrator::queryVisibility(const Ray& ray, float tMax) {
    std::vector<Prim*> prims = m_scene.getPrims();
    for(Prim* p : prims)
    {
        Intersection lightInter = p->Intersect(ray);

        if(lightInter.hit && ((lightInter.t + C_EPS) < tMax))
        {
            return false;
        }
    }
    return true;
}

Intersection Integrator::intersectLights(const Ray& ray)
{
    std::vector<Light*> lights = m_scene.getLights();
    Intersection rInter = Intersection::NoHit;
    for(Light* l : lights)
    {
        Intersection inter = l->intersect(ray);
        if(inter.hit && inter.t < rInter.t)
        {
            rInter = inter;
        }
    }

    return rInter;
}
