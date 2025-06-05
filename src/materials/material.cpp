#include "materials/material.h"

Vec3 Material::getAlbedo() const
{
    return m_albedo;
}

float Material::getIor() const
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

const Bsdf& Material::getBsdf() const
{
    return m_bsdf;
}

const std::string& Material::getName() const
{
    return m_name; 
};

Vec3 LightMaterial::getRadiantExitance() const
{
    return m_color * m_intensity; // [W / m2]
}

