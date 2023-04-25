#pragma once

#include <Defines/Defines.h>
#include <PCH/pch.h>

#include <cmath>

namespace Clunk
{
    class Vec4
    {
    public:
    
        f32 x, y , z, w;

        Vec4(const f32 X = 0.0f, const f32 Y = 0.0f, const f32 Z = 0.0f, const f32 W = 1.0f) : x(X), y(Y), z(Z), w(W) {};
        Vec4(const f32 values[4])
        {
            x = values[0];
            y = values[1];
            z = values[2];
            w = values[3];
        }

////////////////////////////////////////////////////////////

        f32& operator[](u8 i)
        {
            return ((&x)[i]);
        }

        const f32& operator[](u8 i) const
        {
            return ((&x)[i]);
        }

/////////////////////////////////////////////////////////

        bool operator==(const Vec4& Other)
        {
            return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
        }

        const bool operator==(const Vec4& Other) const
        {
            return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
        }

        void operator=(const Vec4& Other)
        {
            x = Other.x;
            y = Other.y;
            z = Other.z;
            w = Other.w;
        }

////////////////////////////////////////////////////////////

        Vec4 operator+(const f32 Val)
        {
            Vec4 v;
            v.x = x + Val;
            v.y = y + Val;
            v.z = z + Val;
            // v = w + Val;
            return v;
        }

        const Vec4 operator+(const f32 Val) const
        {
            Vec4 v;
            v.x = x + Val;
            v.y = y + Val;
            v.z = z + Val;
            // v = w + Val;
            return v;
        }

        void operator+=(const f32 Val)
        {
            x += Val;
            y += Val;
            z += Val;
            // w += Val;
        }

        Vec4 operator+(const Vec4& Other)
        {
            Vec4 v;
            v.x = x + Other.x;
            v.y = y + Other.y;
            v.z = z + Other.z;
            // v.w = z + Other.w;
            return v;
        }

        const Vec4 operator+(const Vec4& Other) const
        {
            Vec4 v;
            v.x = x + Other.x;
            v.y = y + Other.y;
            v.z = z + Other.z;
            // v.w = w + Other.w;
            return v;
        }

        void operator+=(const Vec4& Other)
        {
            x += Other.x;
            y += Other.y;
            z += Other.z;
            // w += Other.w;
        }


///////////////////////////////////////////////////

        Vec4 operator-(const f32 Val)
        {
            Vec4 v;
            v.x = x - Val;
            v.y = y - Val;
            v.z = z - Val;
            // v.w = w - Val;
            return v;
        }

        const Vec4 operator-(const f32 Val) const
        {
            Vec4 v;
            v.x = x - Val;
            v.y = y - Val;
            v.z = z - Val;
            // v.w = w - Val;
            return v;
        }

        void operator-=(const f32 Val)
        {
            x -= Val;
            y -= Val;
            z -= Val;
            // w -= Val;
        }

        Vec4 operator-(const Vec4& Other)
        {
            Vec4 v;
            v.x = x - Other.x;
            v.y = y - Other.y;
            v.z = z - Other.z;
            // v.w = w - Other.w;
            return v;
        }

        const Vec4 operator-(const Vec4& Other) const
        {
            Vec4 v;
            v.x = x - Other.x;
            v.y = y - Other.y;
            v.z = z - Other.z;
            // v.w = w - Other.w;
            return v;
        }

        void operator-=(const Vec4& Other)
        {
            x -= Other.x;
            y -= Other.y;
            z -= Other.z;
            // w -= Other.w;
        }

//////////////////////////////////////////////

    Vec4 operator*(f32 Val)
    {
        Vec4 v;
        v.x = x * Val;
        v.y = y * Val;
        v.z = z * Val;
        // v.w = w * Val;
        return v;
    }

    const Vec4 operator*(f32 Val) const
    {
        Vec4 v;
        v.x = x * Val;
        v.y = y * Val;
        v.z = z * Val;
        // v.w = w * Val;
        return v;
    }

    void operator*=(f32 Val)
    {
        x += Val;
        y += Val;
        z += Val;
        // w += Val;
    }

///////////////////////////////////////////////

    void operator/=(f32 Val)
    {
        Val = 1.0f / Val;
        x *= Val;
        y *= Val;
        z *= Val;
        // w *= Val;
    }

    Vec4 operator/(f32 Val)
    {
        Vec4 v;
        Val = 1.0f / Val;
        v.x = x * Val;
        v.y = y * Val;
        v.z = z * Val;
        // v.y = w * Val;
        return v;
    }

    const Vec4 operator/(f32 Val) const
    {
        Vec4 v;
        Val = 1.0f / Val;
        v.x = x * Val;
        v.y = y * Val;
        v.z = z * Val;
        // v.w = w * Val;
        return v;
    }

//////////////////////////////////////////////

        f32 Length()
        {
            return std::sqrt(x*x + y*y + z*z);
        }

        const f32 Length() const
        {
            return std::sqrt(x*x + y*y + z*z);
        }

        f32 LengthSquared()
        {
            return x*x + y*y + z*z;
        }

        const f32 LengthSquared() const
        {
            return x*x + y*y + z*z;
        }

        void Normalize()
        {
            // TODO: Test this
            *this /= Length();
        }

        Vec4 GetNormal()
        {
            // TODO: Test this
            return *this / Length();
        }

        static Vec4 Normalize(const Vec4& Vec)
        {
            return Vec / Vec.Length();
        }

        const Vec4 GetNormal() const
        {
            // TODO: Test this
            return *this / Length();
        }

        f32 DotProduct(const Vec4& Other)
        {
            return (x * Other.x + y * Other.y + z * Other.z);
        }

        static f32 DotProduct(const Vec4& V1, const Vec4& V2)
        {
            return (V1.x * V2.x + V1.y * V2.y + V1.z * V2.z);
        }

        Vec4 CrossProduct(const Vec4& Other)
        {
            // TODO: update for Vec4 from Vec3
            Vec4 v(
                ((y * Other.z) - (z * Other.y)),
                ((z * Other.x) - (x * Other.z)),
                ((x * Other.y) - (y * Other.x))
            );
            return v;
        }

        static Vec4 CrossProduct(const Vec4& V1, const Vec4& V2)
        {
            // TODO: update for Vec4 from Vec3
            Vec4 v(
                V1.y * V2.z - V1.z * V2.y,
                V1.z * V2.x - V1.x * V2.z,
                V1.x * V2.y - V1.y * V2.x
            );
            return v;
        }

    };
}