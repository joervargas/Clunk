#pragma once

#include <PCH/pch.h>
#include <math.h>

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

#include "Mat3.h"
#include "Mat4.h"
#include "Quat.h"
#include "Transform3d.h"

namespace Clunk::Math
{

    const f32 PI = 3.14159265358979323846f;
    // const double PI =   3.14159265358979323846;

    inline f32 DegToRad(const f32 Degrees)
    {
        return Degrees * (PI/180.f);
        // return Degrees * 0.0174533f;
    }

    inline f32 RadToDeg(const f32 Radians)
    {
        return Radians * (180.f/PI);
        // return Radians * 57.2958f;
    }

    template<typename T>
    T Clamp(T Val, T Min, T Max)
    {
        if(Val < Min) return Min;
        if(Val > Max) return Max;
        return Val;
    }

}