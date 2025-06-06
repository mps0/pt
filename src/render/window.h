#pragma once

#include "pixel.h"

#include <MiniFB.h>
#include <cstdint>

class Window
{
  public:
    Window(uint32_t width, uint32_t height);

    bool     update();
    bool     waitAndSync();
    Pixel    readPixel(uint32_t i, uint32_t j);
    void     writePixel(uint32_t i, uint32_t j, Pixel v);
    uint32_t getHeight() { return m_height; }
    uint32_t getWidth() { return m_width; }

  private:
    uint64_t getBufIdx(uint32_t i, uint32_t j);

    std::vector<uint32_t> m_buf;
    uint32_t              m_width;
    uint32_t              m_height;
    bool                  m_init;

    mfb_window* m_win;
};
