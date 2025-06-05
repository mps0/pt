#include "photonmap/photonmap.h"
#include "materials/material.h"
#include "materials/bsdf.h"
#include "render/prim.h"

void PhotonMap::tracePhotons(const Scene& scene, const uint32_t numPhotons)
{
    std::vector<Light*> lights = scene.getLights();
    for(Light* l : lights)
    {
        float invNumPhotons = 1.0f / numPhotons;
        for(uint32_t i = 0; i < numPhotons; ++i)
        {
            Photon photon = l->shootPhoton(invNumPhotons);

            if(tracePhoton(scene, photon, 0))
            {
                m_photons.push_back(photon);
            }
        }
    }

    m_kdtree.createTree(m_photons);
}

bool PhotonMap::tracePhoton(const Scene& scene, Photon& photon, uint32_t depth, float eta_i)
{
    if(depth > 5)
    {
        return false;
    }
    Ray photonRay = Ray(photon.wPos, photon.wDir);

    // intersect scene
    std::vector<Prim*> prims = scene.getPrims();
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
        return false;
    }

    const Bsdf& bsdf = rInter.mat->getBsdf();
    if(bsdf.getFlags() & Bsdf::DIFFUSE)
    {
        // update photon pos
        photon.wPos = rInter.hitPoint;

        return true;
    }
    else
    {
        Vec3 n = dot(rInter.normal, -photonRay.d) > 0.f ? rInter.normal : -rInter.normal;

        BsdfSample sample = bsdf.sample(rInter.hitPoint, -photonRay.d, n, eta_i, rInter.mat->getIor());

        photon.wDir = sample.s.wo;
        photon.wPos = sample.s.wP;

        return tracePhoton(scene, photon, sample.s.ior, depth + 1);
    }
}

std::vector<Photon> PhotonMap::getInterPhotons(uint32_t N, const Intersection& inter) const
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
