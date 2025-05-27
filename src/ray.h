#pragma once

#include "vec.h"

//template <typename T>
class Ray
{
public:
    struct Payload
    {
        float ior;
        Vec3 throughput;
        float depth;
        bool specOnlyPath;

        Payload() : ior(1.0f), throughput(Vec3(1.0f)), depth(0.0f), specOnlyPath(true) {}
        Payload(float _ior, const Vec3& _throughput, float _depth, bool _specOnlyPath) : ior(_ior), throughput(_throughput), depth(_depth), specOnlyPath(_specOnlyPath) {}
    };
    Ray() {}
    Ray(const Vec3& origin, const Vec3& dir) : o(origin), d(dir) {}
    Ray(const Vec3& origin, const Vec3& dir, Payload payload) : o(origin), d(dir), p(payload) {}
    Ray(const Vec3& origin, const Vec3& dir, float ior, Vec3 throughput, float depth, bool specOnlyPath) : o(origin), d(dir), p(ior, throughput, depth, specOnlyPath) {}

    Vec3 o;
    Vec3 d;
    Payload p;
};


