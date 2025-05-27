#pragma once

#include "integrator.h"

class NEEIntegrator : public Integrator
{
public:
    NEEIntegrator(const Scene& scene, uint32_t maxDepth) : Integrator(scene, maxDepth) {}

private:
    virtual Vec3 computeLo(const Ray& ray, const Intersection& inter) override;
Vec3 computeDirectLigting(const Ray& ray, const Intersection& inter);

};
