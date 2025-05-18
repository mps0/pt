#pragma once

#include "vec.h"

struct Photon
{
    Vec3 flux;
    Vec3 wPos;
    //Vec3 normal;
    float phi;
    float theta;

    Photon() {}
    Photon(Vec3 _wPos)  : wPos(_wPos) {}
};
