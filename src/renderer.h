#pragma once

#include <mutex>
#include <queue>
#include <atomic>

#include "window.h"
#include "scene.h"
#include "integrator.h"
#include "photonmap.h"

class Renderer
{
public: Renderer(Window& win, Scene& scene, Integrator& integrator, PhotonMap& photonmap, float fov, uint32_t samplesPerPixel) : m_win(win), m_scene(scene), m_integrator(integrator), m_photonmap(photonmap), m_fov(fov), m_samplesPerPixel(samplesPerPixel)
    {
        m_accum = reinterpret_cast<Vec3*>(calloc(m_win.getHeight() * m_win.getWidth(), sizeof(Vec3)));
    }

    ~Renderer()
    {
        free(m_accum);
    }

    void render();

private:
    struct Tile
    {
        uint32_t i0;
        uint32_t j0;
        uint32_t width;
        uint32_t height;
    };

    void renderPixel(const Tile& tile);
    void worker();


    Window& m_win;
    Scene& m_scene;
    Integrator& m_integrator;
    PhotonMap& m_photonmap;
    float m_fov;
    uint32_t m_samplesPerPixel;
    Vec3* m_accum;
    bool m_usePhotonMap;

    std::mutex m_queueMutex;
    std::mutex m_winMutex;
    std::queue<Tile> m_tileQueue;
};
