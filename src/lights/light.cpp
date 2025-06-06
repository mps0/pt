#include "lights/light.h"
#include "utils/defs.h"
#include "utils/sampler.h"
#include "utils/utils.h"
#include <memory>

Light::Sample PointLight::sample()
{
    return {Vec2(0.f), 1.0f, m_pos};
}

Photon PointLight::shootPhoton(float weight)
{
    Photon photon;
    photon.wDir = Sampler::the().sampleUniformSphere().s;
    photon.wPos = m_pos;
    photon.flux = getTotalPower() * weight;

    return photon;
}

Vec3 PointLight::evalLe(Sample sample, Vec3 p)
{
    Vec3 v = sample.wP - p;

    return (m_mat->getRadiantExitance() * C_INV_4PI) / std::max(dot(v, v), C_EPS) * sample.invPDF;
}

Vec3 PointLight::getTotalPower()
{
    // hacky ofc, since point lights have no area
    return m_mat->getRadiantExitance();
}

std::shared_ptr<Prim> RectangleLight::getPrim()
{
    return m_rect;
}

Light::Sample RectangleLight::sample()
{
    RandomSample<Vec2> surfaceSample = m_rect->sampleSurface();
    Vec3 wP = surfaceSample.s.x * m_rect->getU() + surfaceSample.s.y * m_rect->getV() +
              m_rect->getCenter();
    return {surfaceSample.s, surfaceSample.invPdf, wP};
}

Photon RectangleLight::shootPhoton(float weight)
{
    Sample lightSample = sample();

    Ray   photonRay;
    float invPdf;
    makeHemisphereRay(lightSample.wP + C_EPS * m_rect->getNormal(), m_rect->getNormal(), photonRay,
                      invPdf);

    Photon photon;
    photon.wPos = photonRay.o;
    photon.wDir = photonRay.d;
    photon.flux = getTotalPower() * weight;

    return photon;
}

Vec3 RectangleLight::evalLe(Sample sample, Vec3 p)
{
    Vec3  v    = sample.wP - p;
    float cosL = -dot(m_rect->getNormal(), normalize(v));

    if (cosL > 0.0f)
    {
        // Since the rectangle light has a cosine-weighted emiitance
        // and E[cos] = 2 / pi
        // we weight the radiant existance by pi / 2
        // so the total power emitted is correct
        Vec3 cosWeightedExitance = m_mat->getRadiantExitance() * C_PI * 0.5f;
        return cosWeightedExitance * cosL * C_INV_2PI * sample.invPDF / std::max(dot(v, v), C_EPS);
    }

    return Vec3(0.f);
}

Intersection RectangleLight::intersect(const Ray& ray)
{
    return m_rect->Intersect(ray);
}

Vec3 RectangleLight::getTotalPower()
{
    return m_mat->getRadiantExitance() * m_rect->getArea();
}
