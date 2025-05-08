#include "integrator.h"
#include "defs.h"
#include "sampler.h"
#include "vec.h"

Vec3 Integrator::Intersect(Ray& ray, Scene& scene)
{
    //std::cout << "STARTING" << std::endl;
    return traceRay(ray, scene, 0);
}

Vec3 Integrator::traceRay(const Ray& ray, const Scene& scene, uint32_t depth)
{
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

            //RandomSample<Vec3> samp = Sampler::the().sampleUniformHemisphere();
            RandomSample<Vec3> samp = Sampler::the().sampleCosineHemisphere();

            Ray outRay;
            outRay.o = rInter.hitPoint + C_EPS * rInter.normal;

            Vec3 helper = std::abs(rInter.normal.z) < 0.99f ? Vec3(0.0f, 0.0f, 1.0f) : Vec3(0.0, 1.0f, 0.0f);
            Vec3 T = normalize(cross(helper, rInter.normal));
            Vec3 B = normalize(cross(rInter.normal, T));

            Vec3 sampWS = samp.sample.x * T + samp.sample.y * B + samp.sample.z * rInter.normal;

            outRay.d = sampWS;

            //bounce
            return albedo * traceRay(outRay, scene, depth + 1) * samp.InvPDF * dot(rInter.normal, outRay.d);
        }
    }

    return Lo;
}
