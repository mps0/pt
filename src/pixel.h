#pragma once

#include <cstdint>

// why does minifb expect gbra? ordering
struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    // assumes float values in interval [0.f, 1.0]
    Pixel(float _r, float _g, float _b, float _a)
    {
        r = static_cast<uint8_t>(_r * 255.f);
        g = static_cast<uint8_t>(_g * 255.f);
        b = static_cast<uint8_t>(_b * 255.f);
        a = static_cast<uint8_t>(_a * 255.f);
    }
};
