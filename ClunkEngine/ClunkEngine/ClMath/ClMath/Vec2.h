#pragma once


#include <PCH/pch.h>

#include <cmath>
#include "HashHelper.h"

namespace Clunk
{
    class Vec2
    {
    public:
    
        union
        {
            struct
            {
                f32 x, y;
            };

            struct
            {
                f32 u, v;
            };
            struct
            {
                f32 r, g;
            };

            f32 vec[2];
        };

        Vec2(const f32 X = 0.0f, const f32 Y = 0.0f) : x(X), y(Y) {};
        Vec2(const f32 Values[2])
        {
            x = Values[0];
            y = Values[1];
        }

///////////////////////////////////////////////////////////

        f32& operator[](u8 i)
        {
            return ((&x)[i]);
        }

        const f32& operator[](u8 i) const
        {
            return ((&x)[i]);
        }

/////////////////////////////////////////////////////////

        bool operator==(const Vec2& Other)
        {
            return x == Other.x && y == Other.y;
        }

        const bool operator==(const Vec2& Other) const
        {
            return x == Other.x && y == Other.y;
        }

        void operator=(const Vec2& Other)
        {
            x = Other.x;
            y = Other.y;
        }

/////////////////////////////////////////////////////////

        Vec2 operator+(const f32 Val)
        {
            Vec2 v;
            v = x + Val;
            v = y + Val;
            return v;
        }

        const Vec2 operator+(const f32 Val) const
        {
            Vec2 v;
            v = x + Val;
            v = y + Val;
            return v;
        }

        void operator+=(const f32 Val)
        {
            x += Val;
            y += Val;
        }

        Vec2 operator+(const Vec2& Other)
        {
            Vec2 v;
            v.x = x + Other.x;
            v.y = y + Other.y;
            return v;
        }

        const Vec2 operator+(const Vec2& Other) const
        {
            Vec2 v;
            v.x = x + Other.x;
            v.y = y + Other.y;
            return v;
        }

        void operator+=(const Vec2& Other)
        {
            x += Other.x;
            y += Other.y;
        }


///////////////////////////////////////////////////

        Vec2 operator-(const f32 Val)
        {
            Vec2 v;
            v = x - Val;
            v = y - Val;
            return v;
        }

        const Vec2 operator-(const f32 Val) const
        {
            Vec2 v;
            v = x - Val;
            v = y - Val;
            return v;
        }

        void operator-=(const f32 Val)
        {
            x -= Val;
            y -= Val;
        }

        Vec2 operator-(const Vec2& Other)
        {
            Vec2 v;
            v.x = x - Other.x;
            v.y = y - Other.y;
            return v;
        }

        const Vec2 operator-(const Vec2& Other) const
        {
            Vec2 v;
            v.x = x - Other.x;
            v.y = y - Other.y;
            return v;
        }

        void operator-=(const Vec2& other)
        {
            x -= other.x;
            y -= other.y;
        }

//////////////////////////////////////////////

        Vec2 operator*(f32 Val)
        {
            Vec2 v;
            v.x = x * Val;
            v.y = y * Val;
            return v;
        }

        const Vec2 operator*(f32 Val) const
        {
            Vec2 v;
            v.x = x * Val;
            v.y = y * Val;
            return v;
        }

        void operator*=(f32 Val)
        {
            x += Val;
            y += Val;
        }

///////////////////////////////////////////////

        void operator/=(f32 Val)
        {
            Val = 1.0f / Val;
            x *= Val;
            y *= Val;
        }

        Vec2 operator/(f32 Val)
        {
            Vec2 v;
            Val = 1.0f / Val;
            v = x * Val;
            v = y * Val;
            return v;
        }

        const Vec2 operator/(f32 Val) const
        {
            Vec2 v;
            Val = 1.0f / Val;
            v = x * Val;
            v = y * Val;
            return v;
        }

//////////////////////////////////////////////

        f32 Length()
        {
            return std::sqrt(x*x + y*y);
        }

        const f32 Length() const
        {
            return std::sqrt(x*x + y*y);
        }

        f32 LengthSquared()
        {
            return x*x + y*y;
        }

        const f32 LengthSquared() const
        {
            return x*x + y*y;
        }

        void Normalize()
        {
            // TODO: Test this
            *this /= Length();
        }

        Vec2 GetNormal()
        {
            // TODO: Test this
            return *this / Length();
        }

        static Vec2 Normalize(const Vec2& Vec)
        {
            return Vec / Vec.Length();
        }

        const Vec2 GetNormal() const
        {
            // TODO: Test this
            return *this / Length();
        }

        f32 DotProduct(const Vec2& Other)
        {
            return (x * Other.x + y * Other.y);
        }

        static f32 DotProduct(const Vec2& V1, const Vec2& V2)
        {
            return (V1.x * V2.x + V1.y * V2.y);
        }

        f32 CrossProduct(const Vec2& Other)
        {
            return x * Other.y - y * Other.x;
        }

        static f32 CrossProduct(const Vec2& V1, const Vec2& V2)
        {
            return V1.x * V2.y - V1.y * V2.x;
        }

    };
}

namespace std
{
    template<> struct hash<Clunk::Vec2>
    {
        size_t operator()(const Clunk::Vec2& V) const
        {
            // size_t xHash = hash<f32>()(V.x);
            // size_t yHash = (hash<f32>()(V.y) << 1);
            // size_t zHash = (hash<f32>()(V.z) << 1);
            // return (xHash ^ yHash >> 1) ^ zHash;
            return ((hash<f32>()(V.x)
                    ^ (hash<f32>()(V.y) << 1)) >> 1);
        }
    };
}