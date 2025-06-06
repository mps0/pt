#pragma once

#include "materials/material.h"
#include "render/intersection.h"
#include "utils/sampler.h"
#include "utils/ray.h"

class Prim
{
public:
    Prim(Material* mat, std::string name) : m_mat(mat), m_name(name) {}
    virtual Intersection Intersect(const Ray& ray) = 0;
    virtual RandomSample<Vec2> sampleSurface() = 0;
    virtual float getArea() const = 0;

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
    virtual RandomSample<Vec2> sampleSurface() override;
    virtual float getArea() const override;

private:
    Vec3 m_center;
    float m_radius;
};

class Rectangle : public Prim
{
public:
    Rectangle(Material* mat, Vec3 center, Vec3 u, Vec3 v, std::string name = "Rectangle") :
        Prim(mat, name),
        m_center(center),
        m_u(normalize(u)),
        m_v(normalize(v)),
        m_normal(normalize(cross(m_u, m_v))),
        m_width(length(u)),
        m_height(length(v))
        {
        }

    virtual Intersection Intersect(const Ray& ray) override;
    virtual RandomSample<Vec2> sampleSurface() override;
    virtual float getArea() const override;
    RandomSample<Vec2> sampleSurfaceWS();
    Vec3 getU() const;
    Vec3 getV() const;
    Vec3 getCenter() const;
    Vec3 getNormal() const;

private:

    Vec3 m_center;
    Vec3 m_u;
    Vec3 m_v;
    Vec3 m_normal;
    float m_width;
    float m_height;
};
