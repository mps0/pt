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

    void print()
    {
        std::cout << "PHOTON flux: "; ::print(flux); std::cout << std::endl;
        std::cout << "PHOTON wPos: "; ::print(wPos); std::cout << std::endl;
    }
};
