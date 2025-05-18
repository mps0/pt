#include "photonmap.h"
#include "material.h"
#include "prim.h"

void PhotonMap::tracePhotons(const Scene& scene, const uint32_t numPhotons)
{
    std::vector<Light*> lights = scene.getLights();
    std::vector<Prim*> prims = scene.getPrims();

    for(uint32_t i = 0; i < numPhotons; ++i)
    {
        for(Light* l : lights)
        {
            // just a single point light for now...

            // sample the light surface
            Light::Sample lightSample = l->sample();

            // sample outgoing direction
            RandomSample<Vec3> sample = Sampler::the().sampleUniformSphere();

            Ray ray;
            ray.d = sample.sample;
            ray.o = lightSample.wP;

            // intersect scene
            Intersection rInter{false, FLT_MAX, Vec3(), Vec3(), nullptr};
            for(Prim* p : prims)
            {
                Intersection inter = p->Intersect(ray);
                if(inter.hit && inter.t < rInter.t)
                {
                    rInter = inter;
                }
            }

            if(!rInter.hit)
            {
                continue;
            }

            // store photon in map
            const LightMaterial* mat = l->getMat();
            Photon photon;
            photon.flux = mat->getFlux();
            photon.wPos = rInter.hitPoint;
            photon.wDir = ray.d;

            //std::cout << "HITPOINT: "; ::print(rInter.hitPoint); std::cout << std::endl;
            //std::cout << "PUSHING BACK PHOTON" << std::endl;
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

    //std::cout << "HEAP FINAL" << std::endl;
    //heap.print();

    //std::cout << "PHOTONS[0} FINAL" << std::endl;
    //std::cout << "photons[0] pos: "; ::print(photons[0].wPos); std::cout << std::endl;
    //std::cout << "m_tree[idx].pos: "; ::print(m_photons[hh[0].first].wPos); std::cout << std::endl;

    return photons;
}
