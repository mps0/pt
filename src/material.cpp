#include "material.h"


Vec3 Material::evalLe()
{
    return Vec3(0.f);
}

Vec3 Material::getAlbedo()
{
    return m_albedo;
}

uint32_t Material::getFlags()
{
    return m_flags;
}

float Material::getIor()
{
    return m_ior;
}

const std::string& Material::getName(){ 
    return m_name; 
};

Vec3 LambertianMaterial::evalBrdf(Vec3 wo, Vec3 wi, Vec3 p)
{
    return m_color * C_INV_PI;
}

Vec3 LightMaterial::evalBrdf(Vec3 wo, Vec3 wi, Vec3 p)
{
    return 0.0f;
}

Vec3 LightMaterial::evalLe()
{
    return m_color * m_intensity;
}

Vec3 SpecularMaterial::evalBrdf(Vec3 wo, Vec3 wi, Vec3 p)
{
    return Vec3(0.0f);
}

Vec3 GlassMaterial::evalBrdf(Vec3 wo, Vec3 wi, Vec3 p)
{
    return Vec3(0.0f);
}
