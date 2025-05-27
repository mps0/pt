#include "photonintegrator.h"
#include "bsdf.h"

Vec3 PhotonIntegrator::computeLo(const Ray& ray, const Intersection& inter)
{
    Vec3 Lo(0.f);
    // emission
    if(inter.mat->getType() == Material::LIGHT && ray.p.specOnlyPath)
    {
        float cosL = dot(inter.normal, -ray.d);

        if(cosL > 0.0f)
        {
            Vec3 Le = inter.mat->getRadiantExitance() * C_INV_2PI * cosL;
            Lo = Lo + Le;
        }
    }

    if(inter.mat->getBsdf().getFlags() & Bsdf::DIFFUSE)
    {
        // evaluate photons
        std::vector<Photon> photons = m_photonmap.getInterPhotons(m_N, inter);

        Vec3 photonAccum;
        uint32_t num = 0;
        for(Photon& p : photons)
        {
            photonAccum += p.flux;
            ++num;
        }

        if(num > 0)
        {
            Vec3 flux = photonAccum;

            Vec3 furthestP = photons.back().wPos;
            Vec3 xp = furthestP - inter.hitPoint;
            float radiusSquared = dot(xp, xp);
            float area = C_PI * radiusSquared;
            Vec3 irradiance = flux / area;

            Vec3 radiance = irradiance * inter.mat->getAlbedo() * C_INV_PI; //TODO
            Lo = Lo + radiance;
        }
    }

    return Lo;
}
