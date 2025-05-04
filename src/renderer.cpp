#include "renderer.h"


Pixel Renderer::Intersect(Ray& ray, Scene& scene)
{
    const std::vector<std::unique_ptr<Prim>>& prims = scene.getPrims();
    Intersection rInter{false, FLT_MAX, Vec3(), Vec3(), nullptr};
    for(const std::unique_ptr<Prim>& p : prims)
    {
        Intersection inter = p->Intersect(ray);
        if(inter.hit && inter.t < rInter.t)
        {
            rInter = inter;
        }
    }

    if(rInter.hit && (rInter.mat != nullptr))
    {
        Vec3 Lo;
        //Vec3 albedo = rInter.mat->eval();
        //Vec3 color = Vec3(0.f, 0.f, 0.f);

        //// sample light
        //const std::vector<std::unique_ptr<Light>>& lights = scene.getLights();
        //for(const std::unique_ptr<Light>& l : lights)
        //{
        //    color = color + albedo *  l->eval(rInter.hitPoint);
        //}

        //return Pixel(color.x, color.y, color.z, 1.f);
        //Vec3 albedo = rInter.mat->eval();
        //return Pixel(albedo.x, albedo.y, albedo.z, 1.f);
        //
        //
        //emissive
        if(rInter.mat->getFlags() & Material::EMISSIVE)
        {
            Vec3 Le = rInter.mat->evalLe(rInter.hitPoint, ray.d);

            Lo = Lo + Le;
        }

        return Pixel(Lo, 1.f);
    }

    return Pixel(0.f, 0.f, 0.f, 1.f);
}

