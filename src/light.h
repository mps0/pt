#pragma once

#include "vec.h"

class Light
{
public:
    virtual Vec3 eval(Vec3 p) = 0;
};

class PointLight : public Light
{
public:
    PointLight(Vec3 pos, float intensity, Vec3 color) :
        m_pos(pos),
        m_intensity(intensity),
        m_color(color)
    {}

    virtual Vec3 eval(Vec3 p) override
    {
        Vec3 v = m_pos - p;
        return (m_intensity / dot(v, v)) * m_color;
    }

private:
    Vec3 m_pos;
    Vec3 m_color;
    float m_intensity;
};
