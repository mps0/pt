#include <cmath>

#include "integrator.h"
#include "defs.h"
#include "material.h"
#include "prim.h"
#include "ray.h"
#include "sampler.h"
#include "vec.h"

Vec3 Integrator::intersect(Ray& ray, Scene& scene, PhotonMap& photonMap)
{
    return traceRay(ray, scene, Vec3(1.f), 0, photonMap);
}

Vec3 Integrator::traceRay(const Ray& ray, const Scene& scene, Vec3 throughput, uint32_t depth, PhotonMap& photonMap, float ior)
{
    Vec3 Lo = Vec3(0.0f);
    if(depth > m_maxDepth)
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

    // photon map enabled

    //TODO
    bool usePhotonMap = true;
    if(usePhotonMap)
    {
        // emission
        if(rInter.mat->getFlags() & Material::EMISSIVE && depth == 0)
        {
            Lo = rInter.mat->evalLe();
        }
        else
        {
            // evaluate photons
            std::vector<Photon> photons = photonMap.getInterPhotons(10, rInter);

            Vec3 photonAccum;
            for(Photon& p : photons)
            {
                photonAccum += p.flux;
            }

            Vec3 flux = photonAccum / photons.size();
            Vec3 furthestP = photons.back().wPos;
            Vec3 xp = furthestP - rInter.hitPoint;
            float radiusSquared = dot(xp, xp);
            float area = C_PI * radiusSquared;

            Vec3 irradiance = flux / area;

            Vec3 radiance = irradiance / C_2PI;

            Lo = radiance;
        }
    }
    else
    {
        // emission
        if(rInter.mat->getFlags() & Material::EMISSIVE)
        {
            Vec3 Le = rInter.mat->evalLe();
            Lo = depth > 0 ? Lo + 0.5f * Le : Lo + Le;
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
            Vec3 n = rInter.normal;
            if(dot(-ray.d, rInter.normal) < 0.f)
            {
                n = -n;
            }

            if(rInter.mat->getFlags() & Material::REFRACTS)
            {
                float eta_i = ior;
                float eta_t = rInter.mat->getIor();

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
                Ray specRay(rInter.hitPoint + n * C_EPS, reflectedDir);
                return rInter.mat->getAlbedo() * traceRay(specRay, scene, fresnel * throughput, depth + 1, photonMap, ior);
            }
            else
            {
                Ray glassRay(rInter.hitPoint - n * C_EPS, refractDir);
                return rInter.mat->getAlbedo() * traceRay(glassRay, scene, (1.0f - fresnel) * throughput, depth + 1, photonMap, rInter.mat->getIor());
            }
        }
    }

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
        Lo = Lo + (temp * traceRay(outRay, scene, throughput, depth + 1, photonMap, ior)) * (1.0f / p);
    }
    return Lo;
}

bool Integrator::queryVisibility(const Ray& ray, const Scene& scene, float tMax) {
    std::vector<Prim*> prims = scene.getPrims();
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

Intersection Integrator::intersectLights(const Ray& ray, const Scene& scene)
{
    std::vector<Light*> lights = scene.getLights();
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

Vec3 Integrator::computeDirectLigting(const Ray& ray, const Scene& scene, const Intersection& inter)
{
    std::vector<Light*> lights = scene.getLights();

    std::vector<std::pair<Vec3, float>> LeSamps;

    bool isSpecular = inter.mat->getFlags() & Material::SPECULAR;
    if(!isSpecular)
    {
        for(Light* l : lights)
        {
            Vec3 adjHitPoint = inter.hitPoint + inter.normal * C_EPS;
            Light::Sample lightSample = l->sample();
            Vec3 lightVec = lightSample.wP - adjHitPoint;

            Ray lightRay;
            lightRay.o = adjHitPoint;
            lightRay.d = normalize(lightVec);

            float lightSamplePdf = 0.f;
            // light sampling
            if(queryVisibility(lightRay, scene, length(lightVec)))
            {
                // NEE
                lightSamplePdf = 1.0f / lightSample.invPDF;
                Vec3 Le = l->eval(lightSample, inter.hitPoint) * dot(inter.normal, lightRay.d) * lightSample.invPDF * inter.mat->evalBrdf(lightRay.d, ray.d, inter.hitPoint);

                if(l->getFlags() & Light::INTERSECTABLE)
                {
                    Le = Le * 0.5f;
                }

                LeSamps.emplace_back(Le, lightSamplePdf);
            }

        }
    }
    // brdf sampling
    Ray outRay;
    float brdfPdf = 0.0f;
    Vec3 brdf(0.0f);
    if(inter.mat->getFlags() & Material::SPECULAR)
    {
        outRay.o = inter.hitPoint + C_EPS * inter.normal;
        Vec3 n = inter.normal;
        if(dot(n, ray.d) > 0)
        {
            n = -n;
        }
        outRay.d = reflect(-ray.d, inter.normal);
        brdfPdf = 1.0f;
        brdf = inter.mat->evalBrdf(outRay.d, ray.d, outRay.o);
    }
    else
    {
        float outRayInvPdf;
        makeHemisphereRay(inter.hitPoint, inter.normal, outRay, outRayInvPdf);

        brdfPdf = 1.0f / outRayInvPdf;
        brdf = inter.mat->evalBrdf(outRay.d, ray.d, outRay.o);
    }

    Intersection lightInter = intersectLights(outRay, scene);
    if(lightInter.hit)
    {
        if(queryVisibility(outRay, scene, lightInter.t))
        {
            LeSamps.emplace_back(lightInter.mat->evalLe() * brdf * dot(inter.normal, outRay.d) / brdfPdf, brdfPdf);
        }
    }

    float pdfAccum = 0.0f;
    for(auto s : LeSamps)
    {
        pdfAccum += s.second;
    }

    Vec3 contrib(0.0f);
    if (pdfAccum > 0.f)
    {
        for(auto s : LeSamps)
        {
            float weight = s.second / pdfAccum;
            contrib += weight * s.first;
        }
    }

    return contrib;
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
