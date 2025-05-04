#include "material.h"
#include "vec.h"
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
    scene.addPrim(std::make_unique<Sphere>(&test0, Vec3(-2.0f, 0.f, -3.5f), 0.75f));
    scene.addPrim(std::make_unique<Sphere>(&test1, Vec3(1.0f, 0.0f, -3.5f), 0.75f));
    scene.addPrim(std::make_unique<Rectangle>(&white, Vec3(0.0f, 1.5f, -3.5f), Vec3(0.f, -1.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec2(1.5f, 1.5f)));

    Window win(RES_X, RES_Y);
    Integrator integrator(6);
    Renderer renderer(win, scene, integrator, 5);

    renderer.render();

    do {
        if (!win.update()) {
            break;
        }
    } while(win.waitAndSync());

    return 0;
}
