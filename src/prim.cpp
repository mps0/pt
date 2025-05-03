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

    float t = (-b - std::sqrtf(descrim)) / (2.f * a);

    Vec3 hitPoint = ray.o + t * ray.d;
    Vec3 normal = hitPoint - m_center;

    return {true, t, hitPoint, normal, m_mat};
}

Intersection Plane::Intersect(const Ray& ray)
{
    float denom = dot(ray.d, m_normal);
    if(denom >= 0.f)
    {
        return Intersection::NoHit;
    }

    Vec3 oc = ray. o - m_center;

    float t = -dot(oc, m_normal) / denom;

    Vec3 hitPoint = ray.o + t * ray.d;

    //TODO
    //Vec3 relHitPoint = hitPoint - 
    //for now assume Y is along Z, and X is along X, and normal is straight down
    
    Vec2 relHitPoint = Vec2(m_center.x, m_center.z) -  Vec2(hitPoint.x, hitPoint.z);
    Vec2 halfExtents = 0.5f * m_extents;

    if(std::abs(relHitPoint.x) > halfExtents.x || std::abs(relHitPoint.y) >  halfExtents.y)
    {
        return Intersection::NoHit;
    }
    

    return {true, t, hitPoint, m_normal, m_mat};
}
