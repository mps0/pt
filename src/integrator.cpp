#include "integrator.h"
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
    if (depth > 2)
    {
        return Lo;
    }

    const std::vector<std::unique_ptr<Prim>>& prims = scene.getPrims();
    Intersection rInter{false, FLT_MAX, Vec3(), Vec3(), nullptr};
    for(const std::unique_ptr<Prim>& p : prims)
    {
        Intersection inter = p->Intersect(ray);
        if(inter.hit && inter.t < rInter.t && inter.t > 0.1f)
        {
                //std::cout << "T: " << inter.t << std::endl;
            //if(depth == 1)
            //{
            //    //getchar();
            //}
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
            outRay.o = rInter.hitPoint + 0.01f * rInter.normal;
            //outRay.o = rInter.hitPoint;
            //samp is in tangent space...
            //vect perpinduclar to Normal...
            //dot(N,T) = 0
            //NxTx + NyTy + NzTz = 0
            //Let x = 0.3, y =0 ,3
            //Tz = -(NxTx + NyTy)/(Nz)
            //
            //T = normalize(Tx, Ty, Tz);
            //
            //B =  cross(T, N)
            //
            Vec3 helper = std::abs(rInter.normal.z) < 0.99f ? Vec3(0.0f, 0.0f, 1.0f) : Vec3(0.0, 1.0f, 0.0f);
            Vec3 T = normalize(cross(helper, rInter.normal));
            Vec3 B = normalize(cross(rInter.normal, T));

            Vec3 sampWS;
            sampWS.x = dot(samp.sample, T);
            sampWS.y = dot(samp.sample, rInter.normal);
            sampWS.z = dot(samp.sample, B);

            outRay.d = sampWS;
            //outRay.d = rInter.normal;
            //bounce
            return albedo * traceRay(outRay, scene, depth + 1) * samp.InvPDF * dot(rInter.normal, outRay.d);
        }
    }

    return Lo;
}
