#pragma once

#include <memory>

#include "defs.h"
#include "material.h"
#include "prim.h"
#include "sampler.h"
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

    virtual Sample sample()  override
    {
        return {Vec2(0.f), 1.0f, m_pos};
    }

    virtual Vec3 eval(Sample sample, Vec3 p) override
    {
        Vec3 v = sample.wP - p;

        return  m_mat->evalLe() * (1.0f / (1.0f +  dot(v, v))) * sample.invPDF;
    }

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

    Prim* getPrim() override
    {
        return reinterpret_cast<Prim*>(m_rect);
    }

    virtual Sample sample() override
    {
        RandomSample<Vec2> surfaceSample = m_rect->sampleSurface();
        Vec3 wP = surfaceSample.sample.x * m_rect->m_u + surfaceSample.sample.y * m_rect->m_v + m_rect->m_center;
        return {surfaceSample.sample, surfaceSample.invPDF, wP};
    }

    virtual Vec3 eval(Sample sample, Vec3 p) override
    {
        Vec3 v = sample.wP - p;
        float cosL = -dot(m_rect->m_normal, normalize(v));

        if(cosL > 0.0f)
        {
            return m_mat->evalLe() * (1.0f / (1.0f + dot(v, v))) * sample.invPDF;
        }

        return Vec3(0.f);
    }

private:
    Rectangle* m_rect;
};
