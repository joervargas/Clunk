#pragma once

#include <PCH/pch.h>

#include <cmath>

#include "Vec3.h"

namespace Clunk
{
    template<typename T>
    class TVec4
    {
    public:
    
        union
        {
            struct
            {
                T X, Y, Z, W;
            };
            struct
            {
                T R, G, B, A;
            };
            
            T Data[4];
        };


        TVec4(const T X = T{}, const T Y = T{}, const T Z = T{}, const T W = T{}) : X(X), Y(Y), Z(Z), W(W) {};
        TVec4(const T values[4])
        {
            X = values[0];
            Y = values[1];
            Z = values[2];
            W = values[3];
        }

////////////////////////////////////////////////////////////

        TVec3<T> ToVec3()
        {
            return TVec3<T>(X, Y, Z);
        }

        TVec3<T> ToVec3() const
        {
            return TVec3<T>(X, Y, Z);
        }

////////////////////////////////////////////////////////////

        T& operator[](u8 Index)
        {
            return ((&X)[Index]);
        }

        const T& operator[](u8 Index) const
        {
            return ((&X)[Index]);
        }

/////////////////////////////////////////////////////////

        bool operator==(const TVec4& Other)
        {
            return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
        }

        const bool operator==(const TVec4& Other) const
        {
            return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
        }

        void operator=(const TVec4& Other)
        {
            X = Other.X;
            Y = Other.Y;
            Z = Other.Z;
            W = Other.W;
        }

////////////////////////////////////////////////////////////

        TVec4 operator+(const T Val)
        {
            TVec4 v;
            v.X = X + Val;
            v.Y = Y + Val;
            v.Z = Z + Val;
            v.W = W + Val;
            return v;
        }

        const TVec4 operator+(const T Val) const
        {
            TVec4 v;
            v.X = X + Val;
            v.Y = Y + Val;
            v.Z = Z + Val;
            v.W = W + Val;
            return v;
        }

        void operator+=(const T Val)
        {
            X += Val;
            Y += Val;
            Z += Val;
            W += Val;
        }

        TVec4 operator+(const TVec4& Other)
        {
            TVec4 v;
            v.X = X + Other.X;
            v.Y = Y + Other.Y;
            v.Z = Z + Other.Z;
            v.W = W + Other.W;
            return v;
        }

        const TVec4 operator+(const TVec4& Other) const
        {
            TVec4 v;
            v.X = X + Other.X;
            v.Y = Y + Other.Y;
            v.Z = Z + Other.Z;
            v.W = W + Other.W;
            return v;
        }

        void operator+=(const TVec4& Other)
        {
            X += Other.X;
            Y += Other.Y;
            Z += Other.Z;
            W += Other.W;
        }


///////////////////////////////////////////////////

        TVec4 operator-(const T Val)
        {
            TVec4 v;
            v.X = X - Val;
            v.Y = Y - Val;
            v.Z = Z - Val;
            v.W = W - Val;
            return v;
        }

        const TVec4 operator-(const T Val) const
        {
            Vec4 v;
            v.X = X - Val;
            v.Y = Y - Val;
            v.Z = Z - Val;
            v.W = W - Val;
            return v;
        }

        void operator-=(const T Val)
        {
            X -= Val;
            Y -= Val;
            Z -= Val;
            W -= Val;
        }

        TVec4 operator-(const TVec4& Other)
        {
            TVec4 v;
            v.X = X - Other.X;
            v.Y = Y - Other.Y;
            v.Z = Z - Other.Z;
            v.W = W - Other.W;
            return v;
        }

        const TVec4 operator-(const TVec4& Other) const
        {
            TVec4 v;
            v.X = x - Other.X;
            v.Y = y - Other.Y;
            v.Z = z - Other.Z;
            v.W = w - Other.W;
            return v;
        }

        void operator-=(const TVec4& Other)
        {
            X -= Other.X;
            Y -= Other.Y;
            Z -= Other.Z;
            W -= Other.W;
        }

//////////////////////////////////////////////

    TVec4 operator*(T Val)
    {
        TVec4 v;
        v.X = X * Val;
        v.Y = Y * Val;
        v.Z = Z * Val;
        v.W = W * Val;
        return v;
    }

    const TVec4 operator*(T Val) const
    {
        TVec4 v;
        v.X = X * Val;
        v.Y = Y * Val;
        v.Z = Z * Val;
        v.W = W * Val;
        return v;
    }

    void operator*=(T Val)
    {
        X *= Val;
        Y *= Val;
        Z *= Val;
        W *= Val;
    }

    TVec4 operator*(const TVec4& Other)
    {
        TVec4 v;
        v.X = X * Other.X;
        v.Y = Y * Other.Y;
        v.Z = Z * Other.Z;
        v.W = W * Other.W;
        return v;
    }

    const TVec4 operator*(const TVec4& Other) const
    {
        TVec4 v;
        v.X = X * Other.X;
        v.Y = Y * Other.Y;
        v.Z = Z * Other.Z;
        v.W = W * Other.W;
        return v;
    }

    void operator*=(const TVec4& Other)
    {
        X *= Other.X;
        Y *= Other.Y;
        Z *= Other.Z;
        W *= Other.W;
    }

///////////////////////////////////////////////

    void operator/=(T Val)
    {
        Val = T(1) / Val;
        X *= Val;
        Y *= Val;
        Z *= Val;
        W *= Val;
    }

    TVec4 operator/(T Val)
    {
        TVec4 v;
        Val = T(1) / Val;
        v.X = X * Val;
        v.Y = Y * Val;
        v.Z = Z * Val;
        v.Y = W * Val;
        return v;
    }

    const TVec4 operator/(T Val) const
    {
        TVec4 v;
        Val = T(1) / Val;
        v.X = X * Val;
        v.Y = Y * Val;
        v.Z = Z * Val;
        v.W = W * Val;
        return v;
    }

//////////////////////////////////////////////

        T Length()
        {
            return std::sqrt(X*X + Y*Y + Z*Z);
        }

        const T Length() const
        {
            return std::sqrt(X*X + Y*Y + Z*Z);
        }

        T LengthSquared()
        {
            return X*X + Y*Y + Z*Z;
        }

        const T LengthSquared() const
        {
            return X*X + Y*Y + Z*Z;
        }

        void Normalize()
        {
            // TODO: Test this
            *this /= Length();
        }

        TVec4 GetNormal()
        {
            // TODO: Test this
            return *this / Length();
        }

        static TVec4 Normalize(const TVec4& Vec)
        {
            return Vec / Vec.Length();
        }

        const TVec4 GetNormal() const
        {
            // TODO: Test this
            return *this / Length();
        }

        T DotProduct(const TVec4& Other)
        {
            return (X * Other.X + Y * Other.Y + Z * Other.Z);
        }

        static T DotProduct(const TVec4& V1, const TVec4& V2)
        {
            return (V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z);
        }

        TVec4 CrossProduct(const TVec4& Other)
        {
            // TODO: update for Vec4 from Vec3
            TVec4 v(
                ((Y * Other.Z) - (Z * Other.Y)),
                ((Z * Other.X) - (X * Other.Z)),
                ((X * Other.Y) - (Y * Other.X))
            );
            return v;
        }

        static TVec4 CrossProduct(const TVec4& V1, const TVec4& V2)
        {
            // TODO: update for Vec4 from Vec3
            TVec4 v(
                V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X
            );
            return v;
        }
    };

//     class Vec4
//     {
//     public:
    
//         f32 x, y , z, w;

//         Vec4(const f32 X = 0.0f, const f32 Y = 0.0f, const f32 Z = 0.0f, const f32 W = 1.0f) : x(X), y(Y), z(Z), w(W) {};
//         Vec4(const f32 values[4])
//         {
//             x = values[0];
//             y = values[1];
//             z = values[2];
//             w = values[3];
//         }

// ////////////////////////////////////////////////////////////

//         f32& operator[](u8 i)
//         {
//             return ((&x)[i]);
//         }

//         const f32& operator[](u8 i) const
//         {
//             return ((&x)[i]);
//         }

// /////////////////////////////////////////////////////////

//         bool operator==(const Vec4& Other)
//         {
//             return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
//         }

//         const bool operator==(const Vec4& Other) const
//         {
//             return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
//         }

//         void operator=(const Vec4& Other)
//         {
//             x = Other.x;
//             y = Other.y;
//             z = Other.z;
//             w = Other.w;
//         }

// ////////////////////////////////////////////////////////////

//         Vec4 operator+(const f32 Val)
//         {
//             Vec4 v;
//             v.x = x + Val;
//             v.y = y + Val;
//             v.z = z + Val;
//             v.w = w + Val;
//             return v;
//         }

//         const Vec4 operator+(const f32 Val) const
//         {
//             Vec4 v;
//             v.x = x + Val;
//             v.y = y + Val;
//             v.z = z + Val;
//             v.w = w + Val;
//             return v;
//         }

//         void operator+=(const f32 Val)
//         {
//             x += Val;
//             y += Val;
//             z += Val;
//             w += Val;
//         }

//         Vec4 operator+(const Vec4& Other)
//         {
//             Vec4 v;
//             v.x = x + Other.x;
//             v.y = y + Other.y;
//             v.z = z + Other.z;
//             v.w = w + Other.w;
//             return v;
//         }

//         const Vec4 operator+(const Vec4& Other) const
//         {
//             Vec4 v;
//             v.x = x + Other.x;
//             v.y = y + Other.y;
//             v.z = z + Other.z;
//             v.w = w + Other.w;
//             return v;
//         }

//         void operator+=(const Vec4& Other)
//         {
//             x += Other.x;
//             y += Other.y;
//             z += Other.z;
//             w += Other.w;
//         }


// ///////////////////////////////////////////////////

//         Vec4 operator-(const f32 Val)
//         {
//             Vec4 v;
//             v.x = x - Val;
//             v.y = y - Val;
//             v.z = z - Val;
//             v.w = w - Val;
//             return v;
//         }

//         const Vec4 operator-(const f32 Val) const
//         {
//             Vec4 v;
//             v.x = x - Val;
//             v.y = y - Val;
//             v.z = z - Val;
//             v.w = w - Val;
//             return v;
//         }

//         void operator-=(const f32 Val)
//         {
//             x -= Val;
//             y -= Val;
//             z -= Val;
//             w -= Val;
//         }

//         Vec4 operator-(const Vec4& Other)
//         {
//             Vec4 v;
//             v.x = x - Other.x;
//             v.y = y - Other.y;
//             v.z = z - Other.z;
//             v.w = w - Other.w;
//             return v;
//         }

//         const Vec4 operator-(const Vec4& Other) const
//         {
//             Vec4 v;
//             v.x = x - Other.x;
//             v.y = y - Other.y;
//             v.z = z - Other.z;
//             v.w = w - Other.w;
//             return v;
//         }

//         void operator-=(const Vec4& Other)
//         {
//             x -= Other.x;
//             y -= Other.y;
//             z -= Other.z;
//             w -= Other.w;
//         }

// //////////////////////////////////////////////

//     Vec4 operator*(f32 Val)
//     {
//         Vec4 v;
//         v.x = x * Val;
//         v.y = y * Val;
//         v.z = z * Val;
//         v.w = w * Val;
//         return v;
//     }

//     const Vec4 operator*(f32 Val) const
//     {
//         Vec4 v;
//         v.x = x * Val;
//         v.y = y * Val;
//         v.z = z * Val;
//         v.w = w * Val;
//         return v;
//     }

//     void operator*=(f32 Val)
//     {
//         x *= Val;
//         y *= Val;
//         z *= Val;
//         w *= Val;
//     }

//     Vec4 operator*(const Vec4& Other)
//     {
//         Vec4 v;
//         v.x = x * Other.x;
//         v.y = y * Other.y;
//         v.z = z * Other.z;
//         v.w = w * Other.w;
//         return v;
//     }

//     const Vec4 operator*(const Vec4& Other) const
//     {
//         Vec4 v;
//         v.x = x * Other.x;
//         v.y = y * Other.y;
//         v.z = z * Other.z;
//         v.w = w * Other.w;
//         return v;
//     }

//     void operator*=(const Vec4& Other)
//     {
//         x *= Other.x;
//         y *= Other.y;
//         z *= Other.z;
//         w *= Other.w;
//     }

// ///////////////////////////////////////////////

//     void operator/=(f32 Val)
//     {
//         Val = 1.0f / Val;
//         x *= Val;
//         y *= Val;
//         z *= Val;
//         // w *= Val;
//     }

//     Vec4 operator/(f32 Val)
//     {
//         Vec4 v;
//         Val = 1.0f / Val;
//         v.x = x * Val;
//         v.y = y * Val;
//         v.z = z * Val;
//         // v.y = w * Val;
//         return v;
//     }

//     const Vec4 operator/(f32 Val) const
//     {
//         Vec4 v;
//         Val = 1.0f / Val;
//         v.x = x * Val;
//         v.y = y * Val;
//         v.z = z * Val;
//         // v.w = w * Val;
//         return v;
//     }

// //////////////////////////////////////////////

//         f32 Length()
//         {
//             return std::sqrt(x*x + y*y + z*z);
//         }

//         const f32 Length() const
//         {
//             return std::sqrt(x*x + y*y + z*z);
//         }

//         f32 LengthSquared()
//         {
//             return x*x + y*y + z*z;
//         }

//         const f32 LengthSquared() const
//         {
//             return x*x + y*y + z*z;
//         }

//         void Normalize()
//         {
//             // TODO: Test this
//             *this /= Length();
//         }

//         Vec4 GetNormal()
//         {
//             // TODO: Test this
//             return *this / Length();
//         }

//         static Vec4 Normalize(const Vec4& Vec)
//         {
//             return Vec / Vec.Length();
//         }

//         const Vec4 GetNormal() const
//         {
//             // TODO: Test this
//             return *this / Length();
//         }

//         f32 DotProduct(const Vec4& Other)
//         {
//             return (x * Other.x + y * Other.y + z * Other.z);
//         }

//         static f32 DotProduct(const Vec4& V1, const Vec4& V2)
//         {
//             return (V1.x * V2.x + V1.y * V2.y + V1.z * V2.z);
//         }

//         Vec4 CrossProduct(const Vec4& Other)
//         {
//             // TODO: update for Vec4 from Vec3
//             Vec4 v(
//                 ((y * Other.z) - (z * Other.y)),
//                 ((z * Other.x) - (x * Other.z)),
//                 ((x * Other.y) - (y * Other.x))
//             );
//             return v;
//         }

//         static Vec4 CrossProduct(const Vec4& V1, const Vec4& V2)
//         {
//             // TODO: update for Vec4 from Vec3
//             Vec4 v(
//                 V1.y * V2.z - V1.z * V2.y,
//                 V1.z * V2.x - V1.x * V2.z,
//                 V1.x * V2.y - V1.y * V2.x
//             );
//             return v;
//         }

//   };

    typedef TVec4<f32> Vec4;
    typedef TVec4<i32> IVec4;
}