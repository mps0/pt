#include <cstdio>
#include <iostream>

#include<iostream>

#include "material.h"
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
    ColorMaterial green({0.f, 1.f, 0.f});
    ColorMaterial blue({0.f, 0.f, 1.f});

    ColorMaterial test0({0.33f, 1.f, 0.f});
    ColorMaterial test1({1.f, 0.33f, 0.f});

    EmissiveMaterial white(Vec3(0.5f));

    Scene scene;
    //scene.addPrim(std::make_unique<Sphere>(&test0, Vec3(-2.0f, 0.f, -3.5f), 0.75f));
    scene.addPrim(std::make_unique<Sphere>(&test1, Vec3(1.0f, 0.0f, -3.5f), 0.75f));
    scene.addPrim(std::make_unique<Rectangle>(&white, Vec3(0.0f, 1.5f, -3.5f), Vec3(0.f, -1.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(1.5f, 1.5f)));

    Window win(RES_X, RES_Y);
    Renderer  r(6);

    for(uint32_t i = 0; i < RES_Y; ++i)
    {
        std::cout << "ROW: " << i << "/" << RES_Y - 1<< std::endl;
        for(uint32_t j = 0; j < RES_X; ++j)
        {
            float pixelsPerUnitLength = RES_Y;
            float x = (float(j) + 0.5f - 0.5f * RES_X) / pixelsPerUnitLength;
            float y = (float(i) + 0.5f - 0.5f * RES_Y) / pixelsPerUnitLength;
            y *= -1.f;

            Vec3 cam_pos = Vec3(0.f, 0.f, 0.f);
            Vec3 pix_pos = Vec3(x, y, -1.f);

            Ray ray;
            ray.o = Vec3(0.f, 0.f, 0.f);
            ray.d = normalize(pix_pos - cam_pos);

            uint32_t numSamples = 5;
            Vec3 res = Vec3(0.0f);
            for(uint32_t k = 0; k < numSamples ; ++k)
            {
                res = res + r.Intersect(ray, scene);
            }

            res = (1.0f / numSamples) * res;

            win.writePixel(i, j,  Pixel(clampZeroToOne(res), 1.0f));
        }
    }

    do {
        if (!win.update()) {
            break;
        }
    } while(win.waitAndSync());

    return 0;
}
