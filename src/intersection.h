#include <cfloat>
#pragma one

#include "material.h"

struct Intersection
{
    static const Intersection NoHit;

    bool hit;
    float t;
    Vec3 hitPoint;
    Vec3 normal;
    Material* mat;
};

inline const Intersection Intersection::NoHit{false, FLT_MAX, Vec3(), Vec3(), nullptr};
