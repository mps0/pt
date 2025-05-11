#pragma once

#include "defs.h"
#include "vec.h"


class Material
{
public:
    enum : uint32_t
    {
        EMISSIVE = 1
    };

    Material(std::string name, Vec3 albedo) : m_name(name), m_albedo(albedo), m_flags(0) {}
    virtual Vec3 evalBrdf(Vec3 wo, Vec3 wi, Vec3 p) = 0;

    virtual Vec3 evalLe()
    {
        return Vec3(0.f);
    }

    Vec3 getAlbedo()
    {
        return m_albedo;
    }

    const uint32_t getFlags()
    {
        return m_flags;
    }

    const std::string& getName(){ return m_name; };

protected:
    std::string m_name;
    uint32_t m_flags;
    Vec3 m_albedo;
};

class LambertianMaterial : public Material
{
public:
    LambertianMaterial(Vec3 albedo, std::string name = "LambertianMaterial") : Material(name, albedo), m_color(albedo) {}
    virtual Vec3 evalBrdf(Vec3 wo, Vec3 wi, Vec3 p) override
    {
        return m_color * C_INV_PI;
    }

private:
    Vec3 m_color;
};

class LightMaterial : public Material
{
public:
    LightMaterial(Vec3 color, float intensity, std::string name = "LightMaterial") : Material(name, Vec3(0.f)), m_color(color), m_intensity(intensity)
    {
        m_flags |= EMISSIVE;
    }

    virtual Vec3 evalBrdf(Vec3 wo, Vec3 wi, Vec3 p) override
    {
        return 0.0f;
    }

    virtual Vec3 evalLe() override
    {
        return m_color * m_intensity;
    }

private:
    Vec3 m_color;
    float m_intensity;
};
