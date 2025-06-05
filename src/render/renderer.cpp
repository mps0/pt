#include "render/renderer.h"
#include "utils/vec.h"

#include <thread>

void Renderer::render()
{
    // Launch threads
    int numThreads = std::thread::hardware_concurrency();

    uint32_t tileWidth = 10;
    uint32_t tileHeight = 10;
    //uint32_t tileWidth = m_win.getWidth();
    //uint32_t tileHeight = m_win.getHeight();

    uint32_t ii = 0;
    std::vector<Tile> tiles;
    while(ii < m_win.getHeight()) 
    {
        uint32_t jj = 0;
        while(jj < m_win.getWidth())
        {
            uint32_t th = ii + tileHeight > m_win.getHeight() ?  m_win.getHeight() - ii : tileHeight;
            uint32_t tw = jj + tileWidth > m_win.getWidth() ?  m_win.getWidth() - jj : tileWidth;
            tiles.push_back({ii, jj, tw, th});
            jj += tileWidth;
        }
        ii += tileHeight;
    }


    uint32_t sampsAccumed = 0;
    for(uint32_t s = 0; s < m_samplesPerPixel ; ++s)
    {
        for(auto t : tiles)
        {
            m_tileQueue.push(t);
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

        ++sampsAccumed;
        for(uint32_t i = 0 ; i < m_win.getHeight() ; ++i)
        {
            for(uint32_t j = 0 ; j < m_win.getWidth() ; ++j)
            {
                Vec3 weighted = m_accum[i * m_win.getWidth() + j] * (1.0f / sampsAccumed);
                m_win.writePixel(i, j, Pixel(clampZeroToOne(weighted), 1.0f));
            }
        }
        std::cout << "UPDATING IMAGE, TOTAL SAMPLES: " << sampsAccumed << std::endl;
        m_win.update();
    }

    do {
        if (!m_win.update()) {
            break;
        }
    } while(m_win.waitAndSync());

    std::cout << "Rendering complete.\n";
}

void Renderer::renderPixel(const Tile& tile)
{
    for (uint32_t i = tile.i0; i < tile.i0 + tile.height; ++i) {
        for (uint32_t j = tile.j0; j < tile.j0 + tile.width; ++j) {

            float pixelsPerUnitLength = 0.5f * m_win.getWidth() / std::tan(m_fov);
            float xOffset = 0.5f;
            float yOffset = 0.5f;

            float x = (float(j) + xOffset - 0.5f * m_win.getWidth()) / pixelsPerUnitLength;
            float y = (float(i) + yOffset - 0.5f * m_win.getHeight()) / pixelsPerUnitLength;
            y *= -1.f;

            Vec3 cam_pos = Vec3(0.f, 0.f, 0.f);
            Vec3 pix_pos = Vec3(x, y, -1.f);

            Ray ray;
            ray.o = Vec3(0.f, 0.f, 0.f);
            ray.d = normalize(pix_pos - cam_pos);

            m_accum[i * m_win.getWidth() + j] = m_accum[i * m_win.getWidth() + j] + m_integrator.intersect(ray);
        }
    }
}

void Renderer::worker()
{
    while (true) {
        Tile tile;
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            if (m_tileQueue.empty())
            {
                return;
            }
            
            tile = m_tileQueue.front();
            m_tileQueue.pop();
        }

        //std::cout << "REMAINING TILES: " << m_tileQueue.size() << std::endl;
        renderPixel(tile);
    }
}
