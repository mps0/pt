#pragma once

#include <cfloat>
#include <cmath>
#include "ray.h"

class Prim
{
public:
    virtual std::pair<bool, float> Intersect(const Ray& ray) = 0;
};

class Sphere : public Prim
{
public:
    Sphere(Vec3 center, float radius) :
        m_center(center),
        m_radius(radius) {}

    virtual std::pair<bool, float> Intersect(const Ray& ray) override;

private:
    Vec3 m_center;
    float m_radius;
};
