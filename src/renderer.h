#pragma once

#include <mutex>
#include <queue>

#include "window.h"
#include "scene.h"
#include "integrator.h"

class Renderer
{
public:
    Renderer(Window& win, Scene& scene, Integrator& integrator, uint32_t samplesPerPixel) : m_win(win), m_scene(scene), m_integrator(integrator), m_samplesPerPixel(samplesPerPixel) {}

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
    uint32_t m_samplesPerPixel;

    std::mutex m_queueMutex;
    std::queue<Tile> m_tileQueue;
};
