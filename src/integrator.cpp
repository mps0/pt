#include "integrator.h"
#include "bsdf.h"
#include "defs.h"
#include "material.h"
#include "prim.h"
#include "ray.h"
#include "sampler.h"
#include "vec.h"
#include "utils.h"
#include <cassert>

Vec3 Integrator::intersect(Ray& ray)
{
    return traceRay(ray);
}

Vec3 Integrator::traceRay(const Ray& ray)
{
    Vec3 Lo = Vec3(0.0f);
    //if(ray.p.depth > 0)
    if(ray.p.depth > m_maxDepth)
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

    Lo = computeLo(ray, rInter);

    const Material* mat = rInter.mat;
    const Bsdf& bsdf = mat->getBsdf();

    //fire new ray
    Vec3 n = dot(rInter.normal, -ray.d) > 0.0f ? rInter.normal : -rInter.normal;
    BsdfSample bsdfSample = bsdf.sample(rInter.hitPoint, -ray.d, n, ray.p.ior, mat->getIor());

    Vec3 contrib = bsdf.computeContrib(bsdfSample) * mat->getAlbedo();

    contrib = (bsdf.getFlags() & Bsdf::SPECULAR) ? contrib : contrib * dot(n, bsdfSample.s.wo);

    Vec3 throughput = ray.p.throughput * contrib;

    float zeta = Sampler::the().sampleUniformUnitInterval() + 0.1f;
    float p = std::max(max(throughput), 1.0f);

    if(zeta < p)
    {
        //bounce
        bool specOnlyPath = ray.p.specOnlyPath && (bsdf.getFlags() & Bsdf::SPECULAR);
        Ray outRay(bsdfSample.s.wP, bsdfSample.s.wo, bsdfSample.s.ior, throughput, ray.p.depth + 1, specOnlyPath);
        Lo = Lo + (contrib * traceRay(outRay)) * (1.0f / p);

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
