#include "render/window.h"

Window::Window(uint32_t width, uint32_t height)
    : m_buf(width * height), m_width(width), m_height(height), m_init(false), m_win(nullptr)
{
    m_win = mfb_open_ex("", width, height, 0x0);
    if (m_win == nullptr)
    {
        std::cout << "Failed to init window!" << std::endl;
        return;
    }
    m_init = true;
}

bool Window::update()
{
    if (mfb_update_ex(m_win, m_buf.data(), m_width, m_height) < 0)
    {
        m_win = nullptr;
        return false;
    }
    return true;
}

bool Window::waitAndSync()
{
    return mfb_wait_sync(m_win);
}

Pixel Window::readPixel(uint32_t i, uint32_t j)
{
    return *reinterpret_cast<Pixel*>(&(m_buf[getBufIdx(i, j)]));
}

void Window::writePixel(uint32_t i, uint32_t j, Pixel v)
{
    uint64_t bufIdx = getBufIdx(i, j);

    uint32_t bgra = v.b;
    bgra |= v.g << 8;
    bgra |= v.r << 16;
    bgra |= v.a << 24;
    m_buf[bufIdx] = bgra;
}

uint64_t Window::getBufIdx(uint32_t i, uint32_t j)
{
    return static_cast<uint64_t>(i) * m_width + static_cast<uint64_t>(j);
}
