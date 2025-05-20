#include "light.h"
#include "defs.h"
#include "sampler.h"


Light::Sample PointLight::sample() 
{
    return {Vec2(0.f), 1.0f, m_pos};
}

Vec3 PointLight::evalLe(Sample sample, Vec3 p)
{
    Vec3 v = sample.wP - p;

    //return  (m_mat->getRadiantExitance() *  C_INV_4PI) / (1.0f + dot(v,v)) * sample.invPDF;
    return  (m_mat->getRadiantExitance() *  C_INV_4PI) / (dot(v,v)) * sample.invPDF;
}

Vec3 PointLight::getTotalPower()
{
    // hacky ofc, since point lights have no area
    return m_mat->getRadiantExitance();
}

Prim* RectangleLight::getPrim()
{
    return reinterpret_cast<Prim*>(m_rect);
}

Light::Sample RectangleLight::sample()
{
    RandomSample<Vec2> surfaceSample = m_rect->sampleSurface();
    Vec3 wP = surfaceSample.sample.x * m_rect->m_u + surfaceSample.sample.y * m_rect->m_v + m_rect->m_center;
    return {surfaceSample.sample, surfaceSample.invPDF, wP};
}

Vec3 RectangleLight::evalLe(Sample sample, Vec3 p)
{
    Vec3 v = sample.wP - p;
    float cosL = -dot(m_rect->m_normal, normalize(v));

    if(cosL > 0.0f)
    {
        // Assumes a cosine-weighted emittance
        return m_mat->getRadiantExitance() * C_INV_2PI * cosL * sample.invPDF;
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
