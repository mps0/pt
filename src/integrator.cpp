#include "integrator.h"
#include "defs.h"
#include "material.h"
#include "prim.h"
#include "ray.h"
#include "sampler.h"
#include "vec.h"
#include <cmath>

Vec3 Integrator::Intersect(Ray& ray, Scene& scene)
{
    return traceRay(ray, scene, Vec3(1.f), 0);
}

Vec3 Integrator::traceRay(const Ray& ray, const Scene& scene, Vec3 throughput, uint32_t depth)
{
    Vec3 Lo = Vec3(0.0f);
    //TODO
    if(depth > 4)
    {
        return Lo;
    }

    std::vector<Prim*> prims = scene.getPrims();
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

    // flip normal if necessary
    Vec3 n = rInter.normal;
    if(dot(-ray.d, rInter.normal) < 0.f)
    {
        n = -n;
    }

    // emission
    if(rInter.mat->getFlags() & Material::EMISSIVE)
    {
        Vec3 Le = rInter.mat->evalLe();
        Lo = depth == 0 ? Lo + Le : Lo + 0.5f * Le; // 0.5f factor to account for NEE
    }


    // NEE
    bool materialReflects = length(rInter.mat->getAlbedo()) > 0.0f ? true : false;
    if(materialReflects)
    {
        Lo += computeDirectLigting(ray, scene, rInter);
    }

    if(rInter.mat->getFlags() & Material::SPECULAR)
    {
        float fresnel = 1.0f; 
        Vec3 refractDir;
        float cos_i;
        float cos_t;
        bool refracted = false;
        if(rInter.mat->getFlags() & Material::REFRACTS)
        {
            float eta_i = 1.0f;
            float eta_t = 1.5f;

            if(dot(-ray.d, rInter.normal) < 0.f)
            {
                std::swap(eta_i, eta_t);
            }

            refracted = refract(-ray.d, n, eta_i, eta_t, refractDir, cos_i, cos_t);

            if(refracted)
            {
                float fresnelParallel = (eta_t * cos_i - eta_i * cos_t)
                    / (eta_t * cos_i + eta_i * cos_t);

                float fresnelPerp = (eta_i * cos_i - eta_t * cos_t)
                    / (eta_i * cos_i + eta_t * cos_t);

                fresnel = 0.5f * (fresnelParallel * fresnelParallel
                        + fresnelPerp * fresnelPerp);
            }
        }

        // reflect or refect
        if(!refracted || fresnel >= Sampler::the().sampleUniformUnitInterval())
        {
            Vec3 reflectedDir = reflect(-ray.d, n);
            Ray specRay(rInter.hitPoint + rInter.normal * C_EPS, reflectedDir);
            return rInter.mat->getAlbedo() * traceRay(specRay, scene, fresnel * throughput, depth + 1);
        }
        else
        {
            Ray glassRay(rInter.hitPoint - n * C_EPS, refractDir);
            return rInter.mat->getAlbedo() * traceRay(glassRay, scene, (1.0f - fresnel) * throughput, depth + 1);
        }
    }

    Ray outRay;
    float outRayInvPdf;
    makeHemisphereRay(rInter.hitPoint, rInter.normal, outRay, outRayInvPdf);

    Vec3 brdf = rInter.mat->evalBrdf(outRay.d, ray.d, rInter.hitPoint);

    Vec3 temp = brdf * throughput * dot(rInter.normal, outRay.d) * outRayInvPdf;
    throughput = throughput * temp;

    float zeta = Sampler::the().sampleUniformUnitInterval();
    float p = max(throughput);

    if(zeta < p)
    {
        //bounce
        Lo = Lo + (temp * traceRay(outRay, scene, throughput, depth + 1)) * (1.0f / p);
    }
    return Lo;
}

bool Integrator::queryVisibility(const Ray& ray, const Scene& scene, float tMax)
{
    std::vector<Prim*> prims = scene.getPrims();
    for(Prim* p : scene.getPrims())
    {
        Intersection lightInter = p->Intersect(ray);

        if(lightInter.hit && (lightInter.t <= tMax))
        {
            return false;
        }
    }
    return true;
}

Vec3 Integrator::computeDirectLigting(const Ray& ray, const Scene& scene, const Intersection& inter)
{
    std::vector<Light*> lights = scene.getLights();

    Vec3 Li = Vec3(0.0f);
    for(Light* l : lights)
    {

        Vec3 adjHitPoint = inter.hitPoint + inter.normal * C_EPS;
        Light::Sample lightSample = l->sample();
        Vec3 lightVec = lightSample.wP - adjHitPoint;

        Ray lightRay;
        lightRay.o = adjHitPoint;
        lightRay.d = normalize(lightVec);

        if(queryVisibility(lightRay, scene, length(lightVec)))
        {
            // NEE
            Li = Li + l->eval(lightSample, inter.hitPoint) * dot(inter.normal, lightRay.d) * lightSample.invPDF * inter.mat->evalBrdf(lightRay.d, ray.d, inter.hitPoint);
        }
    }
    return Li;
}

void Integrator::makeHemisphereRay(const Vec3& o, const Vec3& normal, Ray& outRay, float& invPdf)
{
    RandomSample<Vec3> samp = Sampler::the().sampleCosineHemisphere();
    outRay.o = o + C_EPS * normal;
    Vec3 helper = std::abs(normal.z) < 0.99f ? Vec3(0.0f, 0.0f, 1.0f) : Vec3(0.0, 1.0f, 0.0f);
    Vec3 T = normalize(cross(helper, normal));
    Vec3 B = normalize(cross(normal, T));
    Vec3 sampWS = samp.sample.x * T + samp.sample.y * B + samp.sample.z * normal;
    outRay.d = normalize(sampWS);

    invPdf = samp.invPDF;
}
