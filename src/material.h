#pragma once

#include "vec.h"


class Material
{
public:
    enum : uint32_t
    {
        EMISSIVE = 1
    };

    Material(std::string name) : m_name(name), m_flags(0) {}
    virtual Vec3 eval() = 0;
    virtual Vec3 evalLe(Vec3 p, Vec3 wo)
    {
        return Vec3(0.f);
    }

    const uint32_t getFlags()
    {
        return m_flags;
    }

    const std::string& getName(){ return m_name; };

protected:
    std::string m_name;
    uint32_t m_flags;
};

class ColorMaterial : public Material
{
public:
    ColorMaterial(Vec3 albedo, std::string name = "ColorMaterial") : Material(name), m_color(albedo) {}
    virtual Vec3 eval() override
    {
        return m_color;
    }

private:
    Vec3 m_color;
};

class EmissiveMaterial : public Material
{
public:
    EmissiveMaterial(Vec3 color, std::string name = "EmissiveMaterial") : Material(name), m_color(color) 
    {
        m_flags |= EMISSIVE;
    }
    virtual Vec3 eval() override
    {
        return Vec3(0.0f);
    }

    virtual Vec3 evalLe(Vec3 p, Vec3 wo) override
    {
        return m_color;
    }

private:
    Vec3 m_color;
};
