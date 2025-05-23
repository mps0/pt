#include "neeintegrator.h"
#include "bsdf.h"
#include "defs.h"
#include "utils.h"

Vec3 NEEIntegrator::computeLo(const Ray& ray, Vec3 throughput, const Intersection& inter, uint32_t depth, uint32_t ior) 
{
    Vec3 Lo(0.0f);
    // emission
    if(inter.mat->getType() == Material::LIGHT)
    {
        float cosL = dot(inter.normal, -ray.d);

        if(cosL > 0.0f)
        {
            Vec3 Le = inter.mat->getRadiantExitance() * C_INV_2PI * cosL;
            Lo = depth > 0 ? Lo + 0.5f * Le : Lo + Le;
        }
    }

    // NEE
    if(inter.mat->getBsdf().getFlags() & Bsdf::DIFFUSE)
    {
        Lo += 0.5f * computeDirectLigting(ray, inter);
    }

    return Lo;
}

Vec3 NEEIntegrator::computeDirectLigting(const Ray& ray, const Intersection& inter)
{
    std::vector<Light*> lights = m_scene.getLights();
    std::vector<std::pair<Vec3, float>> LeSamps;
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
        if(queryVisibility(lightRay, length(lightVec)))
        {
            // NEE
            lightSamplePdf = 1.0f / lightSample.invPDF;
            BsdfSample bsdfSample;
            bsdfSample.s.ior = inter.mat->getIor();
            //TODO
            bsdfSample.s.rho =  C_INV_PI;
            bsdfSample.s.wP = inter.hitPoint;
            bsdfSample.s.wo = lightRay.d;

            Vec3 contrib = inter.mat->getBsdf().computeContrib(bsdfSample, inter.normal) * inter.mat->getAlbedo();

            Vec3 Le = l->evalLe(lightSample, inter.hitPoint) * dot(inter.normal, lightRay.d) * lightSample.invPDF * contrib;
            LeSamps.emplace_back(Le, lightSamplePdf);
        }
    }

    // brdf sampling
    BsdfSample bsdfSample = inter.mat->getBsdf().sample(inter.hitPoint, -ray.d, inter.normal);

    Ray outRay(bsdfSample.s.wP, bsdfSample.s.wo);
    Intersection lightInter = intersectLights(outRay);
    if(lightInter.hit)
    {
        if(queryVisibility(outRay, lightInter.t))
        {
            Vec3 v = lightInter.hitPoint - ray.o;
            float cosL = -dot(lightInter.normal, normalize(v));

            if(cosL > 0.0f)
            {
                Vec3 cosWeightedExitance = lightInter.mat->getRadiantExitance() * C_PI * 0.5f;
                    
                Vec3 contrib = inter.mat->getBsdf().computeContrib(bsdfSample, inter.normal) * bsdfSample.invPdf * dot(outRay.d, inter.normal);
                LeSamps.emplace_back(cosWeightedExitance * cosL * C_INV_2PI  * contrib, 1.0f / bsdfSample.invPdf);
            }
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
