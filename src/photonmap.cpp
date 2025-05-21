#include "photonmap.h"
#include "material.h"
#include "prim.h"
#include "utils.h"

void PhotonMap::tracePhotons(const Scene& scene, const uint32_t numPhotons)
{
    std::vector<Light*> lights = scene.getLights();
    std::vector<Prim*> prims = scene.getPrims();
    for(Light* l : lights)
    {
        float invNumPhotons = 1.0f / numPhotons;
        for(uint32_t i = 0; i < numPhotons; ++i)
        {
            Photon photon = l->shootPhoton(invNumPhotons);
            Ray photonRay = Ray(photon.wPos, photon.wDir);

            // intersect scene
            Intersection rInter{false, FLT_MAX, Vec3(), Vec3(), nullptr};
            for(Prim* p : prims)
            {
                Intersection inter = p->Intersect(photonRay);
                if(inter.hit && inter.t < rInter.t)
                {
                    rInter = inter;
                }
            }

            if(!rInter.hit)
            {
                continue;
            }

            // update photon pos
            photon.wPos = rInter.hitPoint;

            m_photons.push_back(photon);
        }
    }

    m_kdtree.createTree(m_photons);
}

std::vector<Photon> PhotonMap::getInterPhotons(uint32_t N, const Intersection& inter)
{
    MaxHeap heap(N);
    m_kdtree.search(inter.hitPoint, heap, 0, 0); 

    std::vector<Photon> photons;

    const std::vector<IdxDist>& hh = heap.getHeap();
    for(auto e : hh)
    {
        photons.push_back(m_kdtree.getNode(e.first).photon);
    }

    return photons;
}
