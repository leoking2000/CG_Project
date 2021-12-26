#pragma once
#include "defines.h"
#define GLM_GTX_rotate_vector
#include <glm/glm.hpp>
#include <glm/ext.hpp>

constexpr f32 PI = 3.14159265f;
constexpr f64 PI_D = 3.1415926535897932;

static inline f32 toRadians(f32 degrees) { return (degrees * 2.0f * PI) / 360.0f; }

template<typename T>
inline T wrap_angle(T theta)
{
    const T modded = fmod(theta, (T)2.0 * (T)PI_D);
    return (modded > (T)PI_D) ?
        (modded - (T)2.0 * (T)PI_D) :
        modded;
}

template<typename T>
inline T Interpolate(const T& a, const T& b, float alpha)
{
    return a + (b - a) * alpha;
}

template<typename T>
inline constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
    return v < lo ? lo : hi < v ? hi : v;
}