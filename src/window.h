#pragma once

#include <cstdint>
#include <iostream>
#include <MiniFB.h>
#include "pixel.h"

class Window
{
public:
    Window(uint32_t width, uint32_t height);
    ~Window();

    bool update();
    bool waitAndSync();
    void writePixel(uint32_t i, uint32_t j, Pixel v);
    uint32_t getHeight() { return m_height; }
    uint32_t getWidth() { return m_width;  }

private:

    uint64_t getBufIdx(uint32_t i, uint32_t j);

    uint32_t* m_buf;
    uint32_t m_width;
    uint32_t m_height;
    bool m_init;

    mfb_window* m_win;
};
