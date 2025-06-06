#pragma once

#include "utils/vec.h"

struct Photon
{
    Vec3 flux;
    Vec3 wPos;
    Vec3 wDir;

    std::string print()
    {
        std::string ret;
        ret += "flux: " + ::print(flux) + "\n";
        ret += "wPos: " + ::print(wPos) + "\n";
        ret += "wDir: " + ::print(wPos) + "\n";

        return ret;
    }
};
