#include "light.h"
#include "sampler.h"


Light::Sample PointLight::sample() 
{
    return {Vec2(0.f), 1.0f, m_pos};
}

Vec3 PointLight::eval(Sample sample, Vec3 p)
{
    Vec3 v = sample.wP - p;

    return  m_mat->evalLe() * (1.0f / (1.0f +  dot(v, v))) * sample.invPDF;
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

Vec3 RectangleLight::eval(Sample sample, Vec3 p)
{
    Vec3 v = sample.wP - p;
    float cosL = -dot(m_rect->m_normal, normalize(v));

    if(cosL > 0.0f)
    {
        return m_mat->evalLe() * (1.0f / (1.0f + dot(v, v))) * sample.invPDF;
    }

    return Vec3(0.f);
}
