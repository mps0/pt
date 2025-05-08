#include "material.h"
#include "vec.h"
#include "window.h"
#include "renderer.h"
#include "scene.h"
#include "prim.h"

//TODO FOV?

constexpr float ASPECT_RATIO = 1.66f;
constexpr float RES_Y = 600.f;
constexpr float RES_X = RES_Y * ASPECT_RATIO;
constexpr uint32_t SAMPLES_PER_PIXEL = 250000;
constexpr uint32_t MAX_DEPTH = 5;

int main()
{
    ColorMaterial red({1.f, 0.f, 0.f});
    ColorMaterial green({0.f, 1.f, 0.f});
    ColorMaterial blue({0.f, 0.f, 1.f});

    ColorMaterial grey({0.25f, 0.25f, 0.25f});
    ColorMaterial darkGrey({0.1f, 0.1f, 0.1f});
    ColorMaterial lightBrown({0.78f, 0.64f, 0.52f});

    ColorMaterial test0({0.33f, 1.f, 0.f});
    ColorMaterial test1({1.f, 0.33f, 0.f});

    EmissiveMaterial white(Vec3(3.0f));

    Scene scene;
    scene.addPrim(std::make_unique<Sphere>(&test0, Vec3(-1.0f, -1.f, -5.0f), 0.5f));
    scene.addPrim(std::make_unique<Sphere>(&test1, Vec3(0.75f, -0.75f, -5.0f), 0.75f));

    //left wall
    scene.addPrim(std::make_unique<Rectangle>(&grey, Vec3(-1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, -1.f), Vec3(0.f, 1.f, 0.f), 3.0f, 4.0f));

    //back wall
    scene.addPrim(std::make_unique<Rectangle>(&grey, Vec3(0.0f, 0.f, -6.5f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), 3.0f, 4.f));

    //right wall
    scene.addPrim(std::make_unique<Rectangle>(&grey, Vec3(1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, 1.f), Vec3(0.f, 1.f, 0.f), 3.0f, 4.f));

    //floor wall
    scene.addPrim(std::make_unique<Rectangle>(&darkGrey, Vec3(0.0f, -2.0f, -5.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 0.f, -1.f), 3.0f, 4.f));

    //Ceiling wall
    scene.addPrim(std::make_unique<Rectangle>(&lightBrown, Vec3(0.0f, 2.0f, -5.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 0.f, 1.f), 3.0f, 4.f));


    // light
    scene.addPrim(std::make_unique<Rectangle>(&white, Vec3(0.0f, 1.5f, -3.5f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 0.f, 1.f), 0.75f, 0.75f));

    Window win(RES_X, RES_Y);
    Integrator integrator(MAX_DEPTH);
    Renderer renderer(win, scene, integrator, SAMPLES_PER_PIXEL);

    renderer.render();

    return 0;
}
