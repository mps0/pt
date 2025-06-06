#pragma once

#include "photonmap/kdtree.h"
#include "render/intersection.h"
#include "scene/scene.h"
#include "utils/photon.h"
#include <memory>

class PhotonMap
{
  public:
    void tracePhotons(const std::shared_ptr<Scene> scene, const uint32_t numPhotons);

    std::vector<Photon> getInterPhotons(uint32_t N, const Intersection& inter) const;

  private:
    bool tracePhoton(const std::shared_ptr<Scene> scene, Photon& photon, uint32_t depth,
                     float eta_i = 1.0f);

    std::vector<Photon> m_photons;
    KDTree              m_kdtree;
};
