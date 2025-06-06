#include "integrator.h"
#include "lights/light.h"
#include "materials/material.h"
#include "integrators/photonintegrator.h"
#include "integrators/neeintegrator.h"
#include "photonmap/photonmap.h"
#include "utils/utils.h"
#include "utils/vec.h"
#include "render/window.h"
#include "render/renderer.h"
#include "render/scene.h"
#include "render/prim.h"

#include <memory>

constexpr float FOV = 20.f;
constexpr float ASPECT_RATIO = 0.75f;
constexpr float RES_Y = 600.f;
constexpr float RES_X = RES_Y * ASPECT_RATIO;
constexpr uint32_t SAMPLES_PER_PIXEL = 10000;
constexpr uint32_t MAX_DEPTH = 6;
constexpr uint32_t NUM_PHOTONS = 1000;
constexpr uint32_t PHOTONS_PER_SAMPLE = 25;
constexpr uint32_t RENDER_TILE_SIZE = 10;

enum IntegratorSelection : uint32_t
{
    NEE = 0,
    PHOTONMAP = 1
};

int main(int argc, char* argv[])
{
    IntegratorSelection selection;
    if(argc < 2)
    {
        std::cout << "no renderer provided, defaulting to nee!" << std::endl;
        selection = NEE;
    }
    else if (argc == 2)
    {
        std::string input = argv[1];
        if (input == "nee")
        {
            selection = NEE;
        }
        else if(input == "photonmap")
        {
            selection = PHOTONMAP;
        }
        else
        {
            selection = NEE;
            std::cout << "input integrator: " << input << " not recognized, defaulting to nee" << std::endl;
        }
    }
    else
    {
        std::cout << "too many inputs provided!" << std::endl;
        return -1;
    }

    LambertianMaterial red({1.f, 0.f, 0.f});
    LambertianMaterial green({0.f, 1.f, 0.f});
    LambertianMaterial blue({0.f, 0.f, 1.f});

    LambertianMaterial black({0.0f, 0.0f, 0.0f});
    LambertianMaterial lightGrey({0.5f, 0.5f, 0.5f});
    LambertianMaterial grey({0.25f, 0.25f, 0.25f});
    LambertianMaterial darkGrey({0.1f, 0.1f, 0.1f});
    LambertianMaterial lightBrown({0.78f, 0.64f, 0.52f});

    SpecularMaterial mirror({1.0f, 1.0f, 1.0f});
    SpecularMaterial blueishMirror({0.5f, 0.5f, 1.0f});

    GlassMaterial glass({1.0f, 1.0f, 1.0f});
    GlassMaterial greenGlass({0.0f, 1.0f, 0.0f});

    LightMaterial degenLightMat(Vec3(0.0f), 0.0f); 
    LightMaterial whiteLightMat(Vec3(1.0f), 200.f);
    LightMaterial whiteLightMat2(Vec3(1.0f), 400.f);
    LightMaterial yellowLightMat(Vec3(1.0f, 1.0f, 0.0f), 2.5f);
    LightMaterial purpleLightMat(Vec3(1.0f, 0.0f, 1.0f), 2.5f);

    Scene scene;
    // spheres
    Sphere sphere0 = Sphere(&glass, Vec3(0.0f, -1.f, -5.0f), 0.8f);
    //Sphere sphere1 = Sphere(&green, Vec3(-0.6f, -1.00f, -5.5f), 0.8f);
    //Sphere sphere2 = Sphere(&red, Vec3(0.7f, -1.5f, -5.5f), 0.6f);

    // walls
    // left wall
    Rectangle leftWall = Rectangle(&grey, Vec3(-1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, -3.f), Vec3(0.f, 4.f, 0.f));
    // back wall
    Rectangle backWall = Rectangle(&grey, Vec3(0.0f, 0.f, -6.5f), Vec3(3.f, 0.f, 0.f), Vec3(0.f, 4.f, 0.f));
    // right wall
    Rectangle rightWall = Rectangle(&grey, Vec3(1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, 3.f), Vec3(0.f, 4.f, 0.f));
    // floor
    Rectangle floor = Rectangle(&grey, Vec3(0.0f, -2.0f, -5.0f), Vec3(3.f, 0.f, 0.f), Vec3(0.f, 0.f, -3.f));
    // ceiling
    Rectangle ceiling = Rectangle(&lightBrown, Vec3(0.0f, 2.0f, -5.0f), Vec3(3.f, 0.f, 0.f), Vec3(0.f, 0.f, 3.f));

    scene.addPrim(&sphere0);
    scene.addPrim(&leftWall);
    scene.addPrim(&backWall);
    scene.addPrim(&rightWall);
    scene.addPrim(&floor);
    scene.addPrim(&ceiling);
    //scene.addPrim(&sphere1);
    //scene.addPrim(&sphere2);

    // lights
    Rectangle lightPrim = Rectangle(&whiteLightMat, Vec3(0.0f, 1.99f, -5.0f), Vec3(1.f, 0.f, 0.f), Vec3(0.f, 0.f, 1.f));
    RectangleLight rectLight(&whiteLightMat2, &lightPrim);
    PointLight pointLight0(&yellowLightMat, Vec3(-1.0f, 1.99f, -4.5f));
    PointLight pointLight1(&purpleLightMat, Vec3(1.0f, 1.99f, -6.0f));
    PointLight pointLight2(&whiteLightMat2, Vec3(0.0f, 1.99f, -5.0f));
    scene.addLight(&rectLight);
    //scene.addLight(&pointLight0);
    //scene.addLight(&pointLight2);

    Window win(RES_X, RES_Y);

    PhotonMap photonmap;
    std::unique_ptr<Integrator> integrator;
    switch(selection)
    {
        case NEE:
            {
                integrator = std::make_unique<NEEIntegrator>(scene, MAX_DEPTH);
                break;
            }

        case PHOTONMAP:
            {
                photonmap.tracePhotons(scene, NUM_PHOTONS);
                integrator = std::make_unique<PhotonIntegrator>(scene, MAX_DEPTH, photonmap, PHOTONS_PER_SAMPLE);
                break;
            }
    }

    Renderer renderer(win, scene, *integrator, degToRad(FOV), SAMPLES_PER_PIXEL, RENDER_TILE_SIZE);
    renderer.render();

    return 0;
}
