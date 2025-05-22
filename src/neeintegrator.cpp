#include "neeintegrator.h"
#include "utils.h"

Vec3 NEEIntegrator::computeLo(const Ray& ray, Vec3 throughput, const Intersection& inter, uint32_t depth, uint32_t ior) 
{
    Vec3 Lo(0.0f);
    // emission
    if(inter.mat->getFlags() & Material::EMISSIVE)
    {
        float cosL = dot(inter.normal, -ray.d);

        if(cosL > 0.0f)
        {
            Vec3 Le = inter.mat->getRadiantExitance() * C_INV_2PI * cosL;
            Lo = depth > 0 ? Lo + 0.5f * Le : Lo + Le;
        }
    }

    // NEE
    bool materialReflects = length(inter.mat->getAlbedo()) > 0.0f ? true : false;
    if(materialReflects)
    {
        Lo += 0.5f * computeDirectLigting(ray, inter);
    }
    if(inter.mat->getFlags() & Material::REFLECTS)
    {
        float fresnel = 1.0f; 
        Vec3 refractDir;
        float cos_i;
        float cos_t;
        bool refracted = false;
        Vec3 n = inter.normal;
        if(dot(-ray.d, inter.normal) < 0.f)
        {
            n = -n;
        }

        if(inter.mat->getFlags() & Material::REFRACTS)
        {
            float eta_i = ior;
            float eta_t = inter.mat->getIor();

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
            Ray specRay(inter.hitPoint + n * C_EPS, reflectedDir);
            return inter.mat->getAlbedo() * traceRay(specRay, fresnel * throughput, depth + 1, ior);
        }
        else
        {
            Ray glassRay(inter.hitPoint - n * C_EPS, refractDir);
            return inter.mat->getAlbedo() * traceRay(glassRay, (1.0f - fresnel) * throughput, depth + 1, inter.mat->getIor());
        }
    }

    return Lo;
}

Vec3 NEEIntegrator::computeDirectLigting(const Ray& ray, const Intersection& inter)
{
    std::vector<Light*> lights = m_scene.getLights();

    std::vector<std::pair<Vec3, float>> LeSamps;

    //TODO terrifble flag....
    //bool reflects = inter.mat->getFlags() & Material::REFLECTS;
    bool temp = max(inter.mat->getAlbedo()) > 0.0f;
    if(temp)
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
            if(queryVisibility(lightRay, length(lightVec)))
            {
                // NEE
                lightSamplePdf = 1.0f / lightSample.invPDF;
                Vec3 Le = l->evalLe(lightSample, inter.hitPoint) * dot(inter.normal, lightRay.d) * lightSample.invPDF * inter.mat->evalBrdf(lightRay.d, ray.d, inter.hitPoint);
                LeSamps.emplace_back(Le, lightSamplePdf);
            }
        }
    }

    // brdf sampling
    Ray outRay;
    float brdfPdf = 0.0f;
    Vec3 brdf(0.0f);
    if(inter.mat->getFlags() & Material::REFLECTS)
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
                LeSamps.emplace_back(cosWeightedExitance * cosL * C_INV_2PI  * brdf * dot(inter.normal, outRay.d) / brdfPdf, brdfPdf);
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
