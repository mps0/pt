#pragma once

#include <cstdint>
#include <iostream>

template<uint8_t N>
class Vec
{
public:
    float data[N];

    float& operator[](uint8_t i) { return data[i]; }
};

template <>
class Vec<2>
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

    Vec<2>() : x(0.f), y(0.f) {}
    Vec<2>(float v) : x(v), y(v) {}
    Vec<2>(float _x, float _y) : x(_x), y(_y) {}

    float& operator[](uint8_t i) { return data[i]; }
};

template <>
class Vec<3>
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

    Vec<3>() : x(0.f), y(0.f), z(0.f) {}
    Vec<3>(float v) : x(v), y(v), z(v) {}
    Vec<3>(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    float& operator[](uint8_t i) { return data[i]; }
};

template <>
class Vec<4>
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

    Vec<4>() : x(0.f), y(0.f), z(0.f), w(0.f) {}
    Vec<4>(float v) : x(v), y(v), z(v), w(v) {}
    Vec<4>(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    float& operator[](uint8_t i) { return data[i]; }
};

    template <uint8_t N>
Vec<N> operator+(Vec<N> v, Vec<N> u)
{
    Vec<N> res;
    for(uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] + u[i];
    }

    return res;
}

    template <uint8_t N>
Vec<N> operator-(Vec<N> v, Vec<N> u)
{
    Vec<N> res;
    for(uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] - u[i];
    }

    return res;
}

template <uint8_t N>
Vec<N> operator*(Vec<N> v, Vec<N> u)
{
    Vec<N> res;
    for(uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] * u[i];
    }

    return res;
}

template <uint8_t N>
Vec<N> operator*(Vec<N> v, float t)
{
    Vec<N> res;
    for(uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] * t;
    }

    return res;
}

template <uint8_t N>
Vec<N> operator*(float t, Vec<N> v)
{
    Vec<N> res;
    for(uint8_t i = 0; i < N; ++i)
    {
        res[i] = v[i] * t;
    }

    return res;
}

template <uint8_t N>
float dot(Vec<N> v, Vec<N> u)
{
    float res = 0.f;
    for(uint8_t i = 0; i < N; ++i)
    {
        res += v[i] * u[i];
    }

    return res;
}

template <uint8_t N>
void print(Vec<N> v)
{
    std::cout << "(";
    Vec<N> res;
    for(uint8_t i = 0; i < N; ++i)
    {
        std::string c = i < (N - 1) ? ", " : ")";
        std::cout << v[i] << c;
    }
}

using Vec2 = Vec<2>;
using Vec3 = Vec<3>;
using Vec4 = Vec<4>;
