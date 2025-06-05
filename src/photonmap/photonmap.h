#pragma once

#include "render/scene.h"
#include "render/intersection.h"
#include "utils/photon.h"
#include "photonmap/kdtree.h"

class PhotonMap
{
public:

    void tracePhotons(const Scene& scene, const uint32_t numPhotons);

    std::vector<Photon> getInterPhotons(uint32_t N, const Intersection& inter) const;

private:
    bool tracePhoton(const Scene& scene, Photon& photon, uint32_t depth, float eta_i = 1.0f);

    std::vector<Photon> m_photons;
    KDTree m_kdtree;
};
