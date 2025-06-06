#pragma once

#include "integrators/integrator.h"
#include "render/window.h"
#include "scene/scene.h"
#include <mutex>
#include <queue>

class Renderer
{
  public:
    Renderer(Window& win, Scene& scene, Integrator& integrator, float fov, uint32_t samplesPerPixel,
             uint32_t renderTileSize)
        : m_win(win), m_scene(scene), m_integrator(integrator), m_fov(fov),
          m_samplesPerPixel(samplesPerPixel), m_renderTileSize(renderTileSize),
          m_accum(m_win.getHeight() * m_win.getWidth())
    {
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

    Window&     m_win;
    Scene&      m_scene;
    Integrator& m_integrator;
    float       m_fov;
    uint32_t    m_samplesPerPixel;
    uint32_t    m_renderTileSize;

    std::vector<Vec3> m_accum;
    std::mutex        m_queueMutex;
    std::mutex        m_winMutex;
    std::queue<Tile>  m_tileQueue;
};
