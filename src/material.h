#pragma once

#include "vec.h"


class Material
{
public:
    virtual Vec3 eval() = 0;

private:

};

class ColorMaterial : public Material
{
public:
    ColorMaterial(Vec3 albedo) : m_color(albedo) {}
    virtual Vec3 eval() override
    {
        return m_color;
    }

private:
    Vec3 m_color;
};

//class LambertianMaterial : public MateriakjVd
