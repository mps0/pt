#include "integrator.h"
#include "defs.h"
#include "material.h"
#include "prim.h"
#include "sampler.h"
#include "vec.h"

Vec3 Integrator::Intersect(Ray& ray, Scene& scene)
{
    return traceRay(ray, scene, Vec3(1.f), 0);
}

Vec3 Integrator::traceRay(const Ray& ray, const Scene& scene, Vec3 throughput, uint32_t depth)
{
    Vec3 Lo = Vec3(0.0f);
    //if(depth > 1)
    //{
    //    return Lo;
    //}

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
        std::vector<Light*> lights = scene.getLights();
        for(Light* l : lights)
        {

            Vec3 adjHitPoint = rInter.hitPoint + rInter.normal * C_EPS;
            Light::Sample lightSample = l->sample();
            Vec3 lightVec = lightSample.wP - adjHitPoint;

            Ray lightRay;
            lightRay.o = adjHitPoint;
            lightRay.d = normalize(lightVec);

            float tLight = length(lightVec);
            bool inShadow = false;
            for(Prim* p : prims)
            {
                Intersection lightInter = p->Intersect(lightRay);

                if(lightInter.hit && (lightInter.t < tLight))
                {
                    inShadow = true;
                    break;
                }
            }

            if(!inShadow)
            {
                // NEE
                Lo = Lo + l->eval(lightSample, rInter.hitPoint) * dot(rInter.normal, lightRay.d) * lightSample.invPDF * rInter.mat->evalBrdf(lightRay.d, ray.d, rInter.hitPoint);
            }
        }
    }


    RandomSample<Vec3> samp = Sampler::the().sampleCosineHemisphere();
    Ray outRay;
    outRay.o = rInter.hitPoint + C_EPS * rInter.normal;
    Vec3 helper = std::abs(rInter.normal.z) < 0.99f ? Vec3(0.0f, 0.0f, 1.0f) : Vec3(0.0, 1.0f, 0.0f);
    Vec3 T = normalize(cross(helper, rInter.normal));
    Vec3 B = normalize(cross(rInter.normal, T));
    Vec3 sampWS = samp.sample.x * T + samp.sample.y * B + samp.sample.z * rInter.normal;
    outRay.d = normalize(sampWS);

    Vec3 brdf = rInter.mat->evalBrdf(outRay.d, ray.d, rInter.hitPoint);
    throughput = throughput * brdf;

    float zeta = Sampler::the().sampleUniformUnitInterval();

    if(zeta <= length(throughput))
    {
        //bounce
        return  Lo + brdf * traceRay(outRay, scene, throughput, depth + 1) * samp.invPDF * dot(rInter.normal, outRay.d);
    }

    return Lo;
}
