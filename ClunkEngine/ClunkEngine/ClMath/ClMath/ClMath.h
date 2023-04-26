#pragma once

#include <Defines/Defines.h>
#include <math.h>

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

#include "Mat3.h"
#include "Mat4.h"
#include "Quat.h"
#include "Transform3d.h"

namespace Clunk
{
    namespace Math
    {
        const f32 PI = 3.14159265358979323846f;

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
    }
}