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

    // specular
    if(rInter.mat->getFlags() & Material::SPECULAR)
    {
        Ray specRay;
        specRay.o = rInter.hitPoint + rInter.normal * C_EPS;
        specRay.d = ray.d - 2.0f * dot(ray.d, rInter.normal) * rInter.normal;
        return rInter.mat->getAlbedo() * traceRay(specRay, scene, throughput, depth + 1);
    }

    // NEE
    bool materialReflects = length(rInter.mat->getAlbedo()) > 0.0f ? true : false;
    if(materialReflects)
    {
        Lo = Lo + computeDirectLigting(ray, scene, rInter);
    }

    Ray outRay;
    float outRayInvPdf;
    makeHemisphereRay(rInter.hitPoint, rInter.normal, outRay, outRayInvPdf);

    Vec3 brdf = rInter.mat->evalBrdf(outRay.d, ray.d, rInter.hitPoint);

    Vec3 temp = brdf * throughput * dot(rInter.normal, outRay.d) * outRayInvPdf;
    throughput = throughput * temp;

    float zeta = Sampler::the().sampleUniformUnitInterval();
    float p = std::max(std::min(1.0f, length(throughput)), 0.1f);

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
