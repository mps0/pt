#pragma once

#include "vec.h"

class Ray
{
public:
    Ray() {}
    Ray(Vec3 origin, Vec3 dir) : o(origin), d(dir) {}

    Vec3 o;
    Vec3 d;
};


