#pragma once

#include "utils.h"
#include "vec.h"
#include "sampler.h"

struct OutdirValue
{
    Vec3 wo;
    Vec3 wP;
    Vec3 rho;
    float ior;
};

using BsdfSample = RandomSample<OutdirValue>;

class Bsdf
{
public:

    enum Flag : uint32_t
    {
        BLACKBODY = 1,
        DIFFUSE = 1 << 1,
        SPECULAR = 1 << 2,
        TRANSMISSIVE =  1 << 3
    };

    Bsdf() : m_flags(Flag(0)) {}
    Bsdf(uint32_t flags) : m_flags(flags) {}

    uint32_t getFlags() const;
    BsdfSample sample(const Vec3& wP, const Vec3& wi, const Vec3& normal, const float eta_i = 1.0f, const float eta_t = 1.0f) const;
    Vec3 computeContrib(const BsdfSample sample, const Vec3& normal) const;

private: 
    BsdfSample sampleSpecular(const Vec3& wP, const Vec3& wi, const Vec3& normal, const float ior, const float fresnel) const;
    BsdfSample sampleTransmission(const Vec3& wP, const Vec3& wo, const float ior, const float fresnel) const;
    BsdfSample sampleDiffuse(const Vec3& wP, const Vec3& wi, const Vec3& normal, const float ior) const;

    uint32_t m_flags;
};
