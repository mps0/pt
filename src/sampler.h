#pragma once

#include <random>

#include "defs.h"
#include "vec.h"

template <typename T>
struct RandomSample
{
    T sample;
    float InvPDF;
};


class Sampler
{
public:

    Sampler() :
        m_generator(std::random_device()()),
        m_distrib(std::uniform_real_distribution<>(0.0f, 1.0f))
    {

    }

    static Sampler& the()
    {
        static Sampler singleton;

        return singleton;
    }

    RandomSample<Vec3> sampleUnifrmHemisphere()
    {
        float r0 = m_distrib(m_generator);
        float r1 = m_distrib(m_generator);

        float z = r0;
        float r = std::sqrt(1.f - z * z);
        float phi = 2.0f * 3.141592f * r1;

        return {Vec3(r * std::cos(phi), z, r * std::sin(phi)),  (4.f * C_PI)};
    }

private:

    std::mt19937 m_generator;
    std::uniform_real_distribution<> m_distrib;
};
