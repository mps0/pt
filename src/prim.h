#pragma once

#include <cfloat>
#include <cmath>

#include "ray.h"
#include "material.h"
#include "intersection.h"

class Prim
{
public:
    Prim(Material* mat, std::string name) : m_mat(mat), m_name(name) {}
    virtual Intersection Intersect(const Ray& ray) = 0;

    const std::string& getName(){ return m_name; };

public:
    Material* m_mat;
    std::string m_name;
};

class Sphere : public Prim
{
public:
    Sphere(Material* mat, Vec3 center, float radius, std::string name = "Sphere") :
        Prim(mat, name),
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
    Rectangle(Material* mat, Vec3 center, Vec3 normal, Vec3 up, Vec2 extents, std::string name = "Rectangle") :
        Prim(mat, name),
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
