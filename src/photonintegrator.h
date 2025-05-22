#pragma once

#include "integrator.h"
#include "photonmap.h"


class PhotonIntegrator : public Integrator
{
public:
    PhotonIntegrator(const Scene& scene, uint32_t maxDepth, const PhotonMap& photonmap, uint32_t N) : Integrator(scene, maxDepth), m_photonmap(photonmap), m_N(N) {}
 
private:
    virtual Vec3 computeLo(const Ray& ray, Vec3 throughput, const Intersection& inter, uint32_t depth, uint32_t ior) override;

    const PhotonMap& m_photonmap;
    uint32_t m_N;
};
