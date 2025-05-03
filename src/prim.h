#pragma once

#include <cfloat>
#include <cmath>

#include "ray.h"
#include "material.h"
#include "intersection.h"

class Prim
{
public:
    Prim(Material* mat) : m_mat(mat) {}
    virtual Intersection Intersect(const Ray& ray) = 0;

 public:
    Material* m_mat;
};

class Sphere : public Prim
{
public:
    Sphere(Material* mat, Vec3 center, float radius) :
        Prim(mat),
        m_center(center),
        m_radius(radius) {}

    virtual Intersection Intersect(const Ray& ray) override;

private:
    Vec3 m_center;
    float m_radius;
};
