#include "material.h"

Vec3 Material::getAlbedo()
{
    return m_albedo;
}

float Material::getIor()
{
    return m_ior;
}

Vec3 Material::getRadiantExitance() const
{
    return Vec3(0.0f);
}

Material::Type Material::getType() const
{
    return m_type;
}
const Bsdf& Material::getBsdf()
{
    return m_bsdf;
}

const std::string& Material::getName(){ 
    return m_name; 
};

Vec3 LightMaterial::getRadiantExitance() const
{
    return m_color * m_intensity; // [W / m2]
}

