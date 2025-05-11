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

    virtual Vec3 evalLe();
    Vec3 getAlbedo();
    uint32_t getFlags();
    const std::string& getName();

protected:
    std::string m_name;
    uint32_t m_flags;
    Vec3 m_albedo;
};

class LambertianMaterial : public Material
{
public:
    LambertianMaterial(Vec3 albedo, std::string name = "LambertianMaterial") : Material(name, albedo), m_color(albedo) {}
    virtual Vec3 evalBrdf(Vec3 wo, Vec3 wi, Vec3 p) override;

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

    virtual Vec3 evalBrdf(Vec3 wo, Vec3 wi, Vec3 p) override;

    virtual Vec3 evalLe() override;

private:
    Vec3 m_color;
    float m_intensity;
};
