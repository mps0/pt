#pragma once

#include <cmath>
#include <cstdint>
#include <iostream>

template <uint8_t N> class Vec
{
  public:
    float data[N];

    float& operator[](uint8_t i) { return data[i]; }
};

template <> class Vec<2>
{
  public:
    union
    {
        struct
        {
            float x;
            float y;
        };
        float data[2];
    };

    Vec() : x(0.f), y(0.f) {}
    Vec(float v) : x(v), y(v) {}
    Vec(float _x, float _y) : x(_x), y(_y) {}

    float&       operator[](uint8_t i) { return data[i]; }
    const float& operator[](uint8_t i) const { return data[i]; }
};

template <> class Vec<3>
{
  public:
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float data[3];
    };

    Vec() : x(0.f), y(0.f), z(0.f) {}
    Vec(float v) : x(v), y(v), z(v) {}
    Vec(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float&       operator[](uint8_t i) { return data[i]; }
    const float& operator[](uint8_t i) const { return data[i]; }
};

template <> class Vec<4>
{
  public:
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float data[4];
    };

    Vec() : x(0.f), y(0.f), z(0.f), w(0.f) {}
    Vec(float v) : x(v), y(v), z(v), w(v) {}
    Vec(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    float&       operator[](uint8_t i) { return data[i]; }
    const float& operator[](uint8_t i) const { return data[i]; }
};

template <uint8_t N> Vec<N> operator+(Vec<N> v, Vec<N> u)
{
    Vec<N> res;
    for (uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] + u[i];
    }

    return res;
}

template <uint8_t N> Vec<N> operator-(Vec<N> v, Vec<N> u)
{
    Vec<N> res;
    for (uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] - u[i];
    }

    return res;
}

template <uint8_t N> Vec<N> operator*(Vec<N> v, Vec<N> u)
{
    Vec<N> res;
    for (uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] * u[i];
    }

    return res;
}

template <uint8_t N> Vec<N> operator/(Vec<N> v, Vec<N> u)
{
    Vec<N> res;
    for (uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] / u[i];
    }

    return res;
}

template <uint8_t N> Vec<N> operator*(Vec<N> v, float t)
{
    Vec<N> res;
    for (uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] * t;
    }

    return res;
}

template <uint8_t N> Vec<N> operator/(Vec<N> v, float t)
{
    return v * (1.0f / t);
}

template <uint8_t N> Vec<N> operator*(float t, Vec<N> v)
{
    Vec<N> res;
    for (uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] * t;
    }

    return res;
}

template <uint8_t N> Vec<N> operator-(Vec<N> v)
{
    Vec<N> res;
    for (uint8_t i = 0; i < N; ++i)
    {
        res[i] = -v[i];
    }

    return res;
}

template <uint8_t N> void operator+=(Vec<N>& lhs, Vec<N> rhs)
{
    for (uint8_t i = 0; i < N; ++i)
    {
        lhs[i] = lhs[i] + rhs[i];
    }
}

template <uint8_t N> float dot(Vec<N> v, Vec<N> u)
{
    float res = 0.f;
    for (uint8_t i = 0; i < N; ++i)
    {
        res += v[i] * u[i];
    }

    return res;
}

template <uint8_t N> Vec<N> clampZeroToOne(Vec<N> v)
{
    Vec<N> res(0.f);
    for (uint8_t i = 0; i < N; ++i)
    {
        res[i] = std::min(std::max(v[i], 0.0f), 1.0f);
    }

    return res;
}

template <uint8_t N> float length(Vec<N> v)
{
    return std::sqrt(dot(v, v));
}

template <uint8_t N> Vec<N> normalize(Vec<N> v)
{
    return v * (1.0f / length(v));
}

inline Vec<3> cross(Vec<3> u, Vec<3> v)
{
    return Vec<3>(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}

inline Vec<3> reflect(Vec<3> v, Vec<3> normal)
{
    return -v + 2.0f * dot(v, normal) * normal;
}

inline bool refract(Vec<3> v, Vec<3> normal, float eta_i, float eta_t, Vec<3>& tDir, float& cos_i,
                    float& cos_t)
{
    float eta = eta_t / eta_i;

    Vec<3> dir_i = v;
    cos_i        = dot(dir_i, normal);
    float sin2_i = 1.f - cos_i * cos_i;
    float sin2_t = sin2_i / (eta * eta);

    if (sin2_t >= 1.0f)
    {
        return false;
    }

    cos_t = std::sqrtf(1.f - sin2_t);

    tDir = -dir_i / eta + (cos_i / eta - cos_t) * normal;

    return true;
}

template <uint8_t N> inline float max(Vec<N> v)
{
    float res = v[0];
    for (uint8_t i = 1; i < N; ++i)
    {
        res = std::max(res, v[i]);
    }

    return res;
}

template <uint8_t N> inline float min(Vec<N> v)
{
    float res = v[0];
    for (uint8_t i = 1; i < N; ++i)
    {
        res = std::min(res, v[i]);
    }

    return res;
}

template <uint8_t N> std::string print(Vec<N> v)
{
    std::cout << "(";
    Vec<N>      res;
    std::string ret;
    for (uint8_t i = 0; i < N; ++i)
    {
        std::string c = i < (N - 1) ? ", " : ")";
        ret += std::to_string(v[i]) + c;
    }

    return ret;
}

using Vec2 = Vec<2>;
using Vec3 = Vec<3>;
using Vec4 = Vec<4>;
