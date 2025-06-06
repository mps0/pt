#include "scene/scenes.h"
#include "light.h"
#include "prim.h"

// materials
auto red        = std::make_shared<LambertianMaterial>(Vec3(1.f, 0.f, 0.f));
auto green      = std::make_shared<LambertianMaterial>(Vec3(0.f, 1.f, 0.f));
auto blue       = std::make_shared<LambertianMaterial>(Vec3(0.f, 0.f, 1.f));
auto black      = std::make_shared<LambertianMaterial>(Vec3(0.0f, 0.0f, 0.0f));
auto lightGrey  = std::make_shared<LambertianMaterial>(Vec3(0.5f, 0.5f, 0.5f));
auto grey       = std::make_shared<LambertianMaterial>(Vec3(0.25f, 0.25f, 0.25f));
auto darkGrey   = std::make_shared<LambertianMaterial>(Vec3(0.1f, 0.1f, 0.1f));
auto lightBrown = std::make_shared<LambertianMaterial>(Vec3(0.78f, 0.64f, 0.52f));

auto mirror        = std::make_shared<SpecularMaterial>(Vec3(1.0f, 1.0f, 1.0f));
auto blueishMirror = std::make_shared<SpecularMaterial>(Vec3(0.5f, 0.5f, 1.0f));

auto glass      = std::make_shared<GlassMaterial>(Vec3(1.0f, 1.0f, 1.0f));
auto greenGlass = std::make_shared<GlassMaterial>(Vec3(0.0f, 1.0f, 0.0f));

auto degenLightMat  = std::make_shared<LightMaterial>(Vec3(0.0f), 0.0f);
auto whiteLightMat  = std::make_shared<LightMaterial>(Vec3(1.0f), 200.f);
auto whiteLightMat2 = std::make_shared<LightMaterial>(Vec3(1.0f), 400.f);
auto yellowLightMat = std::make_shared<LightMaterial>(Vec3(1.0f, 1.0f, 0.0f), 2.5f);
auto purpleLightMat = std::make_shared<LightMaterial>(Vec3(1.0f, 0.0f, 1.0f), 2.5f);

// walls
// left wall
auto leftWall = std::make_shared<Rectangle>(grey, Vec3(-1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, -3.f),
                                            Vec3(0.f, 4.f, 0.f));
// back wall
auto backWall = std::make_shared<Rectangle>(grey, Vec3(0.0f, 0.f, -6.5f), Vec3(3.f, 0.f, 0.f),
                                            Vec3(0.f, 4.f, 0.f));
// right wall
auto rightWall = std::make_shared<Rectangle>(grey, Vec3(1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, 3.f),
                                             Vec3(0.f, 4.f, 0.f));
// floor
auto bottom = std::make_shared<Rectangle>(grey, Vec3(0.0f, -2.0f, -5.0f), Vec3(3.f, 0.f, 0.f),
                                          Vec3(0.f, 0.f, -3.f));
// ceiling
auto ceiling = std::make_shared<Rectangle>(lightBrown, Vec3(0.0f, 2.0f, -5.0f), Vec3(3.f, 0.f, 0.f),
                                           Vec3(0.f, 0.f, 3.f));

// lights
auto lightPrim   = std::make_shared<Rectangle>(whiteLightMat, Vec3(0.0f, 1.99f, -5.0f),
                                               Vec3(1.f, 0.f, 0.f), Vec3(0.f, 0.f, 1.f));
auto rectLight   = std::make_shared<RectangleLight>(whiteLightMat2, lightPrim);
auto pointLight0 = std::make_shared<PointLight>(yellowLightMat, Vec3(-1.0f, 1.99f, -4.5f));
auto pointLight1 = std::make_shared<PointLight>(purpleLightMat, Vec3(1.0f, 1.99f, -6.0f));
auto pointLight2 = std::make_shared<PointLight>(whiteLightMat2, Vec3(0.0f, 1.99f, -5.0f));

// spheres
auto glassSphere  = std::make_shared<Sphere>(glass, Vec3(0.0f, -1.f, -5.0f), 0.8f);
auto mirrorSphere = std::make_shared<Sphere>(mirror, Vec3(0.0f, -1.f, -5.0f), 0.8f);
auto greenSphere  = std::make_shared<Sphere>(green, Vec3(-0.6f, -1.00f, -5.5f), 0.8f);
auto redSphere    = std::make_shared<Sphere>(red, Vec3(0.7f, -1.5f, -5.5f), 0.6f);

std::shared_ptr<Scene> makeLitBoxScene()
{
    auto scene = std::make_shared<Scene>();

    // walls
    // left wall
    auto leftWall = std::make_shared<Rectangle>(grey, Vec3(-1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, -3.f),
                                                Vec3(0.f, 4.f, 0.f));
    // back wall
    auto backWall = std::make_shared<Rectangle>(grey, Vec3(0.0f, 0.f, -6.5f), Vec3(3.f, 0.f, 0.f),
                                                Vec3(0.f, 4.f, 0.f));
    // right wall
    auto rightWall = std::make_shared<Rectangle>(grey, Vec3(1.5f, 0.f, -5.0f), Vec3(0.f, 0.f, 3.f),
                                                 Vec3(0.f, 4.f, 0.f));
    // floor
    auto bottom = std::make_shared<Rectangle>(grey, Vec3(0.0f, -2.0f, -5.0f), Vec3(3.f, 0.f, 0.f),
                                              Vec3(0.f, 0.f, -3.f));
    // ceiling
    auto ceiling = std::make_shared<Rectangle>(lightBrown, Vec3(0.0f, 2.0f, -5.0f),
                                               Vec3(3.f, 0.f, 0.f), Vec3(0.f, 0.f, 3.f));

    scene->add(leftWall);
    scene->add(backWall);
    scene->add(rightWall);
    scene->add(bottom);
    scene->add(ceiling);

    scene->add(rectLight);

    return scene;
}

std::shared_ptr<Scene> makeLambertianScene()
{
    auto scene = std::make_shared<Scene>();

    std::shared_ptr<Scene> litBox = makeLitBoxScene();
    scene->add(litBox);

    scene->add(greenSphere);
    scene->add(redSphere);

    return scene;
}

std::shared_ptr<Scene> makeMirrorScene()
{
    auto scene = std::make_shared<Scene>();

    std::shared_ptr<Scene> litBox = makeLitBoxScene();
    scene->add(litBox);

    scene->add(mirrorSphere);

    return scene;
}

std::shared_ptr<Scene> makeGlassScene()
{
    auto scene = std::make_shared<Scene>();

    std::shared_ptr<Scene> litBox = makeLitBoxScene();
    scene->add(litBox);

    scene->add(glassSphere);

    return scene;
}
