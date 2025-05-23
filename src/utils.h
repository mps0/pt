#pragma once

#include "defs.h"
#include "vec.h"
#include "ray.h"
#include "sampler.h"

inline float degToRad(float rad)
{
    return rad * C_PI / 180.f;
}

inline void makeHemisphereRay(const Vec3& o, const Vec3& normal, Ray& outRay, float& invPdf, bool cosineWeighted = true)
{
    RandomSample<Vec3> samp = cosineWeighted ?  Sampler::the().sampleCosineHemisphere() : Sampler::the().sampleUniformHemisphere();

    outRay.o = o + C_EPS * normal;
    Vec3 helper = std::abs(normal.z) < 0.99f ? Vec3(0.0f, 0.0f, 1.0f) : Vec3(0.0, 1.0f, 0.0f);
    Vec3 T = normalize(cross(helper, normal));
    Vec3 B = normalize(cross(normal, T));
    Vec3 sampWS = samp.s.x * T + samp.s.y * B + samp.s.z * normal;
    outRay.d = normalize(sampWS);

    invPdf = samp.invPdf;
}

inline void sampleHemisphereDir(const Vec3& normal, Vec3& outDir, float& invPdf, bool cosineWeighted = true)
{
    RandomSample<Vec3> samp = cosineWeighted ?  Sampler::the().sampleCosineHemisphere() : Sampler::the().sampleUniformHemisphere();

    Vec3 helper = std::abs(normal.z) < 0.99f ? Vec3(0.0f, 0.0f, 1.0f) : Vec3(0.0, 1.0f, 0.0f);
    Vec3 T = normalize(cross(helper, normal));
    Vec3 B = normalize(cross(normal, T));
    Vec3 sampWS = samp.s.x * T + samp.s.y * B + samp.s.z * normal;
    outDir = normalize(sampWS);

    invPdf = samp.invPdf;
}

inline bool refractAndGetFresnel(const Vec<3>& wi, const Vec<3>& normal, const float eta_i, const float eta_t, Vec<3>& wo, float& fresnel)
{
    float cos_i;
    float cos_t;
    bool refracted = refract(wi, normal, eta_i, eta_t, wo, cos_i, cos_t);

    if(refracted)
    {
        float fresnelParallel = (eta_t * cos_i - eta_i * cos_t)
            / (eta_t * cos_i + eta_i * cos_t);

        float fresnelPerp = (eta_i * cos_i - eta_t * cos_t)
            / (eta_i * cos_i + eta_t * cos_t);

        fresnel = 0.5f * (fresnelParallel * fresnelParallel
                + fresnelPerp * fresnelPerp);
    }

    return refracted;
}
