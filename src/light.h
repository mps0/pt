#pragma once

#include "material.h"
#include "prim.h"
#include "vec.h"

class Light
{
public:

    enum : uint32_t
    {
        INTERSECTABLE = 1
    };

    struct Sample
    {
        Vec2 objP; //surface
        float invPDF;
        Vec3 wP;
    };


    Light(LightMaterial* mat) :
        m_mat(mat),
        m_flags(0)
   {}

    virtual Sample sample()  = 0;
    virtual Vec3 eval(Sample sample, Vec3 p) = 0;
    virtual Intersection intersect(const Ray& ray)
    {
        return Intersection::NoHit;
    }

    uint32_t getFlags() 
    {
        return m_flags;
    }

protected:
    Material* m_mat;
    uint32_t m_flags;
};

class PointLight : public Light
{
public:
    PointLight(LightMaterial* mat, Vec3 pos) :
        Light(mat),
        m_pos(pos)
    {}

    virtual Sample sample()  override;
    virtual Vec3 eval(Sample sample, Vec3 p) override;

private:
    Vec3 m_pos;
};

class IntersectableLight: public Light 
{
 public:
    IntersectableLight(LightMaterial* mat) :
        Light(mat)
    {
        m_flags |= INTERSECTABLE;
    }

    virtual Prim* getPrim() = 0;
};

class RectangleLight : public IntersectableLight
{
public:
    RectangleLight(LightMaterial* mat, Rectangle* rect) :
        IntersectableLight(mat),
        m_rect(rect) 
    {
    }

    Prim* getPrim() override;
    virtual Sample sample() override;
    virtual Vec3 eval(Sample sample, Vec3 p) override;
    virtual Intersection intersect(const Ray& ray) override;

private:
    Rectangle* m_rect;
};
