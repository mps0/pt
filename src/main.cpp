#include <cstdio>
#include <iostream>

#include "vec.h"
#include "ray.h"
#include "window.h"
#include "renderer.h"
#include "scene.h"
#include "prim.h"

constexpr float ASPECT_RATIO = 1.66f;
constexpr float RES_Y = 600.f;
constexpr float RES_X = RES_Y * ASPECT_RATIO;

int main()
{
    ColorMaterial red({1.f, 0.f, 0.f});
    ColorMaterial blue({0.f, 0.f, 1.f});

    Scene scene;
    scene.addPrim(std::make_unique<Sphere>(&red, Vec3(-1.0f, 0.f, -3.5f), 0.5f));
    scene.addPrim(std::make_unique<Sphere>(&blue, Vec3(0.0f, 0.5f, -3.5f), 0.3f));

    scene.addLight(std::make_unique<PointLight>(Vec3(0.f, 3.f, -3.5f), 3.f, Vec3(1.f)));

    Window win(RES_X, RES_Y);
    Renderer  r;

    for(uint32_t i = 0; i < RES_Y; ++i)
    {
        for(uint32_t j = 0; j < RES_X; ++j)
        {
            uint32_t idx = (i * RES_X + j);

            float pixelsPerUnitLength = RES_Y;
            float x = (float(j) + 0.5f - 0.5f * RES_X) / pixelsPerUnitLength;
            float y = (float(i) + 0.5f - 0.5f * RES_Y) / pixelsPerUnitLength;
            y *= -1.f;

            Vec3 cam_pos = Vec3(0.f, 0.f, 0.f);
            Vec3 pix_pos = Vec3(x, y, -1.f);

            Ray ray;
            ray.o = Vec3(0.f, 0.f, 0.f);
            ray.d = pix_pos - cam_pos;

            Pixel pix = r.Intersect(ray, scene);
            win.writePixel(i, j,  pix);
        }
    }

    do {
        int state;

        if (!win.update()) {
            break;
        }
    } while(win.waitAndSync());

    return 0;
}
