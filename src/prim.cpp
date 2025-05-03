#include "prim.h"


std::pair<bool, float> Sphere::Intersect(const Ray& ray)
{
    Vec3 oc = ray.o - m_center;
    float a = dot(ray.d, ray.d);
    float b = 2.0f * dot(oc, ray.d);
    float c = dot(oc, oc) - m_radius * m_radius;
    float descrim = b * b - 4.0f * a * c;

    if(descrim < 0.0f)
    {
        return {false, FLT_MAX};
    }

    float t = (-b - std::sqrtf(descrim)) / (2.f * a);

    return {true, t};
}

