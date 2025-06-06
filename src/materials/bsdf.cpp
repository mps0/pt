#include "materials/bsdf.h"
#include "utils/defs.h"
#include "utils/utils.h"

BsdfSample Bsdf::sample(const Vec3& wP, const Vec3& wi, const Vec3& normal, const float eta_i,
                        const float eta_t) const
{
    if (m_flags & BLACKBODY || m_flags == 0)
    {
        return {};
    }

    if (m_flags & DIFFUSE)
    {
        return sampleDiffuse(wP + C_EPS * normal, wi, normal, eta_i);
    }

    if (m_flags & SPECULAR)
    {
        float fresnel = 1.0f;

        if (m_flags & TRANSMISSIVE)
        {
            Vec3 wo;
            bool refracted = refractAndGetFresnel(wi, normal, eta_i, eta_t, wo, fresnel);
            if (refracted && fresnel < Sampler::the().sampleUniformUnitInterval())
            {
                return sampleTransmission(wP - C_EPS * normal, wo, eta_t, fresnel);
            }
        }

        return sampleSpecular(wP + C_EPS * normal, wi, normal, eta_i, fresnel);
    }
    return {};
}

Vec3 Bsdf::computeContrib(const BsdfSample sample) const
{
    if (m_flags & BLACKBODY || m_flags == 0)
    {
        return Vec3(0.f);
    }

    if (m_flags & DIFFUSE)
    {
        return sample.s.rho * sample.invPdf;
    }

    if (m_flags & (SPECULAR | TRANSMISSIVE))
    {
        return sample.s.rho * sample.invPdf;
    }

    return Vec3(0.f);
}

BsdfSample Bsdf::sampleSpecular(const Vec3& wP, const Vec3& wi, const Vec3& normal, const float ior,
                                const float fresnel) const
{
    BsdfSample sample;

    sample.s.wP   = wP;
    sample.s.wo   = reflect(wi, normal);
    sample.s.rho  = fresnel;
    sample.s.ior  = ior;
    sample.invPdf = 1.0f;

    return sample;
}

BsdfSample Bsdf::sampleTransmission(const Vec3& wP, const Vec3& wo, const float ior,
                                    const float fresnel) const
{
    BsdfSample sample;

    sample.s.wP   = wP;
    sample.s.wo   = wo;
    sample.s.rho  = 1.0f - fresnel;
    sample.s.ior  = ior;
    sample.invPdf = 1.0f;

    return sample;
}

BsdfSample Bsdf::sampleDiffuse(const Vec3& wP, const Vec3& wi, const Vec3& normal,
                               const float ior) const
{
    BsdfSample sample;

    Vec3  outDir;
    float invPdf;
    sampleHemisphereDir(normal, outDir, invPdf);

    sample.s.wP   = wP;
    sample.s.wo   = outDir;
    sample.s.rho  = C_INV_PI;
    sample.s.ior  = ior;
    sample.invPdf = invPdf;

    return sample;
}

uint32_t Bsdf::getFlags() const
{
    return m_flags;
}
