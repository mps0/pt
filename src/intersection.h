#pragma one

#include "material.h"

struct Intersection
{
    bool hit;
    float t;
    Vec3 hitPoint;
    Vec3 normal;
    Material* mat;
};
