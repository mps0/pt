#include "prim.h"


Intersection Sphere::Intersect(const Ray& ray)
{
    Vec3 oc = ray.o - m_center;
    float a = dot(ray.d, ray.d);
    float b = 2.0f * dot(oc, ray.d);
    float c = dot(oc, oc) - m_radius * m_radius;
    float descrim = b * b - 4.0f * a * c;

    if(descrim < 0.0f)
    {
        return {false, FLT_MAX, Vec3(), Vec3(), nullptr};
    }

    float t = (-b - std::sqrtf(descrim)) / (2.f * a);

    Vec3 hitPoint = ray.o + t * ray.d;
    Vec3 normal = hitPoint - m_center;

    return {true, t, hitPoint, normal, m_mat};
}

