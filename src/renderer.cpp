#include "renderer.h"

Pixel Renderer::Intersect(Ray& ray, Scene& scene)
{
    const std::vector<std::unique_ptr<Prim>>& prims = scene.getPrims();
    float t = FLT_MAX;
    bool hit = false;
    for(const std::unique_ptr<Prim>& p : prims)
    {
        std::pair<bool, float> inter = p->Intersect(ray);
        if(inter.first)
        {
            hit = true;
            t = inter.second;
        }
    }


    if(hit)
    {
        return Pixel(1.f, 0.f, 0.f, 1.f);
    }
    else
    {
        return Pixel(0.f, 0.f, 0.f, 1.f);
    }

}

