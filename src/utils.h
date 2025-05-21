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
    Vec3 sampWS = samp.sample.x * T + samp.sample.y * B + samp.sample.z * normal;
    outRay.d = normalize(sampWS);

    invPdf = samp.invPDF;
}
