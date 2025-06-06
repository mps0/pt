#pragma once

#include "utils/defs.h"
#include "utils/vec.h"

#include <random>

template <typename T> struct RandomSample
{
    T     s;
    float invPdf;
};

class Sampler
{
  public:
    Sampler()
        : m_generator(std::random_device()()),
          m_distrib(std::uniform_real_distribution<>(0.0f, 1.0f))
    {
    }

    static Sampler& the()
    {
        static Sampler singleton;

        return singleton;
    }

    RandomSample<Vec2> sampleUniformDisc()
    {
        float r0 = m_distrib(m_generator);
        float r1 = m_distrib(m_generator);

        float r   = std::sqrt(r0);
        float phi = r1 * C_2PI;

        return {Vec2(r * std::cos(phi), r * std::sin(phi)), C_PI};
    }

    RandomSample<Vec3> sampleUniformHemisphere()
    {
        float r0 = m_distrib(m_generator);
        float r1 = m_distrib(m_generator);

        float z   = r0;
        float r   = std::sqrt(1.f - z * z);
        float phi = 2.0f * C_PI * r1;

        return {Vec3(r * std::cos(phi), r * std::sin(phi), z), C_2PI};
    }

    RandomSample<Vec3> sampleUniformSphere()
    {
        float r0 = m_distrib(m_generator);
        float r1 = m_distrib(m_generator);

        float z   = 1.0f - 2.0f * r0;
        float r   = std::sqrt(1.f - z * z);
        float phi = 2.0f * C_PI * r1;

        return {Vec3(r * std::cos(phi), r * std::sin(phi), z), C_4PI};
    }

    RandomSample<Vec3> sampleCosineHemisphere()
    {
        // Sample disc
        float r0 = m_distrib(m_generator);
        float r1 = m_distrib(m_generator);

        float r   = std::sqrt(r0);
        float phi = r1 * C_2PI;

        float theta = std::asin(r);

        // Project to hemisphere (Malley's method)
        return {Vec3(r * std::cos(phi), r * std::sin(phi), std::sqrt(1.0f - r * r)),
                C_PI / std::max(std::cos(theta), C_EPS)};
    }

    float sampleUniformUnitInterval() { return m_distrib(m_generator); }

  private:
    std::mt19937                     m_generator;
    std::uniform_real_distribution<> m_distrib;
};
