#pragma once

#include "integrators/integrator.h"
#include "photonmap/photonmap.h"

class PhotonIntegrator : public Integrator
{
  public:
    PhotonIntegrator(const Scene& scene, uint32_t maxDepth, const PhotonMap& photonmap, uint32_t N)
        : Integrator(scene, maxDepth), m_photonmap(photonmap), m_N(N)
    {
    }

  private:
    virtual Vec3 computeLo(const Ray& ray, const Intersection& inter) override;

    const PhotonMap& m_photonmap;
    uint32_t         m_N;
};
