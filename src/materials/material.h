#pragma once

#include "utils/vec.h"
#include "materials/bsdf.h"

class Material
{
public:
    enum Type : uint32_t
    {
        BASE = 0,
        LAMBERTIAN = 1,
        LIGHT = 2,
        SPECULAR = 3,
        GLASS
    };


           Material(std::string name, Vec3 albedo, Bsdf bsdf, Type type, float ior = 1.0f) : m_bsdf(bsdf), m_name(name), m_albedo(albedo), m_type(type), m_ior(ior) {}

           virtual Vec3 getRadiantExitance() const;
           Vec3 getAlbedo() const;
           const std::string& getName() const;
           float getIor() const;
           Type getType() const;
           const Bsdf& getBsdf() const;

protected:
           Bsdf m_bsdf;
           std::string m_name;
           Vec3 m_albedo;
           const Type m_type;
           float m_ior;
};

class LambertianMaterial : public Material
{
public:
    LambertianMaterial(Vec3 albedo, std::string name = "LambertianMaterial") : Material(name, albedo, Bsdf(Bsdf::DIFFUSE), LAMBERTIAN) {}
};

class LightMaterial : public Material
{
public:
    LightMaterial(Vec3 color, float intensity, std::string name = "LightMaterial") : Material(name, Vec3(0.f), Bsdf(Bsdf::BLACKBODY), LIGHT), m_color(color), m_intensity(intensity)
    {
    }

    virtual Vec3 getRadiantExitance() const override;

private:
    Vec3 m_color;
    float m_intensity;
};

class SpecularMaterial : public Material
{
public:
    SpecularMaterial(Vec3 albedo,std::string name = "SpecularMaterial") : Material(name, albedo, Bsdf(Bsdf::SPECULAR), SPECULAR) 
    {
    }
};

class GlassMaterial : public Material
{
public:
    GlassMaterial(Vec3 albedo, std::string name = "GlassMaterial") : Material(name, albedo, Bsdf(Bsdf::SPECULAR | Bsdf::TRANSMISSIVE), GLASS, 1.5f)
    {
    }
};
