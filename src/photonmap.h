#pragma once

#include "scene.h"
#include "photon.h"
#include "kdtree.h"
#include "intersection.h"

class PhotonMap
{
public:

    void tracePhotons(const Scene& scene, const uint32_t numPhotons);

    std::vector<Photon> getInterPhotons(uint32_t N, const Intersection& inter) const;

private:

    std::vector<Photon> m_photons;
    KDTree m_kdtree;
};
