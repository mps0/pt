#pragma once

#include <random>

#include "vec.h"


class Sampler
{
public:


    Sampler() :
        m_generator(std::random_device()()),
        m_distrib(std::uniform_real_distribution<>(0.0f, 1.0f))
    {

    }

    Sampler& the()
    {
        static Sampler singleton;


        return singleton;
    }

    Vec3 sampleUnifrmHemisphere()
    {
        float r0 = m_distrib(m_generator);
        float r1 = m_distrib(m_generator);

        float z = r0;
        float r = std::sqrt(1.f - z * z);
        float phi = 2.0f * 3.141592f * r1;

        return {r * std::cos(phi), r * std::sin(phi), z};
    }

private:

    std::mt19937 m_generator;
    std::uniform_real_distribution<> m_distrib;
};
