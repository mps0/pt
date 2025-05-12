#include "light.h"
#include "material.h"
#include "vec.h"
#include "window.h"
#include "renderer.h"
#include "scene.h"
#include "prim.h"

//TODO FOV?

constexpr float ASPECT_RATIO = 1.66f;
constexpr float RES_Y = 400.f;
constexpr float RES_X = RES_Y * ASPECT_RATIO;
constexpr uint32_t SAMPLES_PER_PIXEL = 1000;
constexpr uint32_t MAX_DEPTH = 5;

int main()
{
    LambertianMaterial red({1.f, 0.f, 0.f});
    LambertianMaterial green({0.f, 1.f, 0.f});
    LambertianMaterial blue({0.f, 0.f, 1.f});

    LambertianMaterial lightGrey({0.5f, 0.5f, 0.5f});
    LambertianMaterial grey({0.25f, 0.25f, 0.25f});
    LambertianMaterial darkGrey({0.1f, 0.1f, 0.1f});
    LambertianMaterial lightBrown({0.78f, 0.64f, 0.52f});

    SpecularMaterial mirror({1.0f, 1.0f, 1.0f});
    SpecularMaterial blueishMirror({0.5f, 0.5f, 1.0f});

    GlassMaterial glass({1.0f, 1.0f, 1.0f});
    GlassMaterial greenGlass({0.0f, 1.0f, 0.0f});

    LightMaterial whiteLightMat(Vec3(1.0f), 30.0f);
    LightMaterial yellowLightMat(Vec3(1.0f, 1.0f, 0.0f), 2.5f);
    LightMaterial purpleLightMat(Vec3(1.0f, 0.0f, 1.0f), 2.5f);

    Scene scene;
    //Spheres
    Sphere sphere0 = Sphere(&glass, Vec3(-1.0f, 0.0f, -5.0f), 0.5f);
    Sphere sphere1 = Sphere(&greenGlass, Vec3(0.5f, -1.5f, -5.0f), 0.75f);
    //Walls
    //left wall
    Rectangle leftWall = Rectangle(&lightGrey, Vec3(-1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, -1.f), Vec3(0.f, 1.f, 0.f), 3.0f, 4.0f);
    //back wall
    Rectangle backWall = Rectangle(&lightGrey, Vec3(0.0f, 0.f, -6.5f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), 3.0f, 4.f);
    //right wall
    Rectangle rightWall = Rectangle(&lightGrey, Vec3(1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, 1.f), Vec3(0.f, 1.f, 0.f), 3.0f, 4.f);

    //floor
    Rectangle floor = Rectangle(&grey, Vec3(0.0f, -2.0f, -5.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 0.f, -1.f), 3.0f, 4.f);
    //ceiling
    Rectangle ceiling = Rectangle(&lightBrown, Vec3(0.0f, 2.0f, -5.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 0.f, 1.f), 3.0f, 4.f);

    scene.addPrim(&sphere0);
    scene.addPrim(&sphere1);
    scene.addPrim(&leftWall);
    scene.addPrim(&backWall);
    scene.addPrim(&rightWall);
    scene.addPrim(&floor);
    scene.addPrim(&ceiling);

    // light
    Rectangle lightPrim = Rectangle(&whiteLightMat, Vec3(0.0f, 1.99f, -5.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 0.f, 1.f), 1.00f, 1.00f);
    RectangleLight rectLight(&whiteLightMat, &lightPrim);
    PointLight pointLight0(&yellowLightMat, Vec3(-1.0f, 1.99f, -4.5f));
    PointLight pointLight1(&purpleLightMat, Vec3(1.0f, 1.99f, -6.0f));
    PointLight pointLight2(&whiteLightMat, Vec3(0.0f, 1.99f, -5.0f));
    scene.addLight(&rectLight);
    scene.addLight(&pointLight0);
    scene.addLight(&pointLight1);
    //scene.addLight(&pointLight2);



    Window win(RES_X, RES_Y);
    Integrator integrator(MAX_DEPTH);
    Renderer renderer(win, scene, integrator, SAMPLES_PER_PIXEL);

    renderer.render();

    return 0;
}
