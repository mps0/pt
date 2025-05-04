#include <thread>

#include "sampler.h"
#include "renderer.h"
#include "vec.h"

void Renderer::render()
{
    // Launch threads
    int numThreads = std::thread::hardware_concurrency();

    uint32_t tileWidth = 10;
    uint32_t tileHeight = 10;

    uint32_t ii = 0;
    while(ii < m_win.getHeight()) 
    {
        uint32_t jj = 0;
        while(jj < m_win.getWidth())
        {
            uint32_t th = ii + tileHeight > m_win.getHeight() ?  m_win.getHeight() - ii : tileHeight;
            uint32_t tw = jj + tileWidth > m_win.getWidth() ?  m_win.getWidth() - jj : tileWidth;
            m_tileQueue.push({ii, jj, tw, th});
            jj += tileWidth;
        }
        ii += tileHeight;
    }

    // Launch threads
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&Renderer::worker, this);
    }

    for (auto& t : threads) 
    {
        t.join();
    }

    std::cout << "Rendering complete.\n";
}

void Renderer::renderPixel(const Tile& tile)
{
    for (uint32_t i = tile.i0; i < tile.i0 + tile.height; ++i) {
        for (uint32_t j = tile.j0; j < tile.j0 + tile.width; ++j) {

            Vec3 res = Vec3(0.0f);
            // TODO clean this up....
            for(uint32_t k = 0; k < m_samplesPerPixel; ++k)
            {
                float pixelsPerUnitLength = m_win.getHeight();
                float xOffset = Sampler::the().sampleUniformUnitInterval();
                float yOffset = Sampler::the().sampleUniformUnitInterval();

                float x = (float(j) + xOffset - 0.5f * m_win.getWidth()) / pixelsPerUnitLength;
                float y = (float(i) + yOffset - 0.5f * m_win.getHeight()) / pixelsPerUnitLength;
                y *= -1.f;

                Vec3 cam_pos = Vec3(0.f, 0.f, 0.f);
                Vec3 pix_pos = Vec3(x, y, -1.f);

                Ray ray;
                ray.o = Vec3(0.f, 0.f, 0.f);
                ray.d = normalize(pix_pos - cam_pos);
                res = res + m_integrator.Intersect(ray, m_scene);
            }

            res = (1.0f / m_samplesPerPixel) * res;

            m_win.writePixel(i, j,  Pixel(clampZeroToOne(res), 1.0f));
        }
    }
}

void Renderer::worker()
{
    while (true) {
        Tile tile;
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            if (m_tileQueue.empty()) return; // no work left
            tile = m_tileQueue.front();
            m_tileQueue.pop();
        }

        std::cout << "REMAINING TILES: " << m_tileQueue.size() << std::endl;
        renderPixel(tile);
    }
}
