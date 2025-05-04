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

class Rectangle : public Prim
{
public:
    Rectangle(Material* mat, Vec3 center, Vec3 normal, Vec3 up, Vec2 extents) :
        Prim(mat),
        m_center(center),
        m_normal(normal),
        m_up(up),
        m_extents(extents)
    {}

    virtual Intersection Intersect(const Ray& ray) override;

private:
    Vec3 m_center;
    Vec3 m_normal;
    Vec3 m_up;
    Vec2 m_extents;
};
