#include "renderer.h"
#include "sampler.h"

Vec3 Renderer::Intersect(Ray& ray, Scene& scene)
{
    return traceRay(ray, scene, 0);
}

Vec3 Renderer::traceRay(const Ray& ray, const Scene& scene, uint32_t depth)
{
    // TODO... pdf?
    Vec3 Lo = Vec3(0.0f);
    if (depth > m_maxDepth)
    {
        return Lo;
    }

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

        //emissive
        if(rInter.mat->getFlags() & Material::EMISSIVE)
        {
            Vec3 Le = rInter.mat->evalLe(rInter.hitPoint, ray.d);

            Lo = Lo + Le;
        }
        else //temp
        {
            Vec3 albedo = rInter.mat->eval();

            RandomSample<Vec3> samp = Sampler::the().sampleUnifrmHemisphere();

            Ray outRay;
            outRay.o = rInter.hitPoint + C_EPS * rInter.normal;
            outRay.d = normalize(rInter.normal + samp.sample);
            //bounce
            return albedo * traceRay(outRay, scene, depth + 1) * samp.InvPDF;
        }
    }

    return Lo;
}
