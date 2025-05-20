#include "prim.h"
#include "vec.h"

Intersection Sphere::Intersect(const Ray& ray)
{
    Vec3 oc = ray.o - m_center;
    float a = dot(ray.d, ray.d);
    float b = 2.0f * dot(oc, ray.d);
    float c = dot(oc, oc) - m_radius * m_radius;
    float descrim = b * b - 4.0f * a * c;

    if(descrim < 0.0f)
    {
        return Intersection::NoHit;
    }

    float sqrtDescrim = std::sqrtf(descrim);

    float temp = b <= -sqrtDescrim ? (-b - sqrtDescrim) : (-b + sqrtDescrim);
    float t = temp / (2.f * a);

    if(t < 0.f)
    {
        return Intersection::NoHit;
    }

    Vec3 hitPoint = ray.o + t * ray.d;
    Vec3 normal = normalize(hitPoint - m_center);

    return {true, t, hitPoint, normal, m_mat};
}

float Sphere::getArea() const
{
    return C_4PI * m_radius * m_radius;
}

Intersection Rectangle::Intersect(const Ray& ray)
{
    float denom = dot(ray.d, m_normal);
    if(denom >= 0.f)
    {
        return Intersection::NoHit;
    }

    Vec3 oc = ray. o - m_center;

    float t = -dot(oc, m_normal) / denom;

    Vec3 hitPoint = ray.o + t * ray.d;

    Vec3 pc = hitPoint - m_center;

    Vec2 hitPointOS;
    hitPointOS.x = dot(pc, m_u);
    hitPointOS.y = dot(pc, m_v);

    if(std::abs(hitPointOS.x) > 0.5f * m_width || std::abs(hitPointOS.y) >  0.5f * m_height)
    {
        return Intersection::NoHit;
    }
    

    return {true, t, hitPoint, m_normal, m_mat};
}

RandomSample<Vec2> Rectangle::sampleSurface()
{
    float x = (Sampler::the().sampleUniformUnitInterval() - 0.5f) * m_width;
    float y = (Sampler::the().sampleUniformUnitInterval() - 0.5f) * m_height;

    return {Vec2(x, y), m_width * m_height};
}
float Rectangle::getArea() const
{
    return m_height * m_width;
}
