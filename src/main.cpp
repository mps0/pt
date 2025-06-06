#include "integrator.h"
#include "integrators/neeintegrator.h"
#include "integrators/photonintegrator.h"
#include "lights/light.h"
#include "materials/material.h"
#include "photonmap/photonmap.h"
#include "render/prim.h"
#include "render/renderer.h"
#include "render/window.h"
#include "scene/scene.h"
#include "scene/scenes.h"
#include "utils/utils.h"
#include "utils/vec.h"

#include <memory>

constexpr float    FOV                = 20.f;
constexpr float    ASPECT_RATIO       = 0.75f;
constexpr float    RES_Y              = 600.f;
constexpr float    RES_X              = RES_Y * ASPECT_RATIO;
constexpr uint32_t SAMPLES_PER_PIXEL  = 10000;
constexpr uint32_t MAX_DEPTH          = 6;
constexpr uint32_t NUM_PHOTONS        = 1000;
constexpr uint32_t PHOTONS_PER_SAMPLE = 25;
constexpr uint32_t RENDER_TILE_SIZE   = 10;

enum IntegratorSelection : uint32_t
{
    NEE       = 0,
    PHOTONMAP = 1
};

int main(int argc, char* argv[])
{
    IntegratorSelection selection;
    if (argc < 2)
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
        else if (input == "photonmap")
        {
            selection = PHOTONMAP;
        }
        else
        {
            selection = NEE;
            std::cout << "input integrator: " << input << " not recognized, defaulting to nee"
                      << std::endl;
        }
    }
    else
    {
        std::cout << "too many inputs provided!" << std::endl;
        return -1;
    }

    std::shared_ptr<Scene> scene = makeGlassScene();

    Window win(RES_X, RES_Y);

    PhotonMap                   photonmap;
    std::unique_ptr<Integrator> integrator;
    switch (selection)
    {
    case NEE:
    {
        integrator = std::make_unique<NEEIntegrator>(*scene, MAX_DEPTH);
        break;
    }

    case PHOTONMAP:
    {
        photonmap.tracePhotons(scene, NUM_PHOTONS);
        integrator =
            std::make_unique<PhotonIntegrator>(*scene, MAX_DEPTH, photonmap, PHOTONS_PER_SAMPLE);
        break;
    }
    }

    Renderer renderer(win, *scene, *integrator, degToRad(FOV), SAMPLES_PER_PIXEL, RENDER_TILE_SIZE);
    renderer.render();

    return 0;
}
