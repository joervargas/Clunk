#pragma once


#include <PCH/pch.h>

#include "Vec3.h"
// #include "Mat4.h"
#include <cmath>


namespace Clunk
{
    class Mat4;

    class Quat
    {
    public:

        union
        {
            struct
            {
                f32 x, y , z, w;
            };
            struct
            {
                Vec3 vec;
                f32 scalar;
            };
            f32 q[4];
        };

        Quat(f32 _x = 0.0f, f32 _y = 0.0f, f32 _z = 0.0f, f32 _w = 0.0f) : 
            x(_x), y(_y), z(_z), w(_w) {}

        Quat(const Vec3& V, f32 S)
        {
            x = V.x; y = V.y; z = V.z; w = S;
        }

        void operator=(const Quat& Other)
        {
            x = Other.x;
            y = Other.y;
            z = Other.z;
            w = Other.w;
        }

        bool operator==(const Quat& Other)
        {
            return (
                fabsf(x - Other.x) <= EPSILON &&
                fabsf(y - Other.y) <= EPSILON &&
                fabsf(z - Other.z) <= EPSILON &&
                fabsf(w - Other.w) <= EPSILON
            );
        }

        bool operator!=(const Quat& Other)
        {
            return !((*this) == Other);
        }

        Quat operator+(const Quat& Other)
        {
            return Quat(x + Other.x, y + Other.y, z + Other.z, w + Other.w);
        }

        const Quat operator+(const Quat& Other) const
        {
            return Quat(x + Other.x, y + Other.y, z + Other.z, w + Other.w);
        }

        Quat operator-(const Quat& Other)
        {
            return Quat(x - Other.x, y - Other.y, z - Other.z, w - Other.w);
        }

        const Quat operator-(const Quat& Other) const
        {
            return Quat(x - Other.x, y - Other.y, z - Other.z, w - Other.w);
        }

        Quat operator-()
        {
            return Quat(-x, -y, -z, -w);
        }

        const Quat operator-() const
        {
            return Quat(-x, -y, -z, -w);
        }

        Quat operator*(f32 Val)
        {
            return Quat(x * Val, y * Val, z * Val, w * Val);
        }

        const Quat operator*(f32 Val) const
        {
            return Quat(x * Val, y * Val, z * Val, w * Val);
        }

        Quat operator*(const Quat& Other)
        {
            Quat result;
            result.scalar = Other.scalar * scalar - Vec3::Dot(Other.vec, vec);
            result.vec = (vec * Other.scalar) + (Other.vec * scalar) + Vec3::Cross(Other.vec, vec);
            return result;
        }

        const Quat operator*(const Quat& Other) const
        {
            Quat result;
            result.scalar = Other.scalar * scalar - Vec3::Dot(Other.vec, vec);
            result.vec = (vec * Other.scalar) + (Other.vec * scalar) + Vec3::Cross(Other.vec, vec);
            return result;
        }

        Vec3 operator*(const Vec3& V)
        {
            return vec * 2.0f * Vec3::Dot(vec, V) + 
                V * (scalar * scalar - Vec3::Dot(vec, vec)) + 
                Vec3::Cross(vec, V) * 2.0f * scalar;
        }

        const Vec3 operator*(const Vec3& V) const
        {
            return vec * 2.0f * Vec3::Dot(vec, V) + 
                V * (scalar * scalar - Vec3::Dot(vec, vec)) + 
                Vec3::Cross(vec, V) * 2.0f * scalar;
        }

        Quat operator^(f32 Val)
        {
            f32 angle = 2.0f * acosf(scalar);
            Vec3 axis = Vec3::Normalize(vec);

            f32 halfCos = cosf(Val * angle * 0.5f);
            f32 halfSin = sinf(Val * angle * 0.5f);

            return Quat(
                axis.x * halfSin,
                axis.y * halfSin,
                axis.z * halfSin,
                halfCos
            );
        }

        static Quat Identity()
        {
            return Quat(0.0f, 0.0f, 0.0f, 1.0f);
        }

        void SetAngleAxis(f32 Angle, const Vec3& Axis);

        static Quat AngleAxis(f32 Angle, const Vec3& Axis);

        static Quat FromTo(const Vec3& From, const Vec3& To);

        static inline Vec3 GetAxis(const Quat& Q)
        {
            return Vec3::Normalize(Vec3(Q.x, Q.y, Q.z));
        }

        inline Vec3 GetAxis()
        {
            return Vec3::Normalize(Vec3(x, y, z));
        }

        static inline f32 GetAngle(const Quat& Q)
        {
            return 2.0f * acosf(Q.w);
        }

        inline f32 GetAngle()
        {
            return 2.0f * acosf(w);
        }

        static inline f32 Dot(const Quat& Q1, const Quat& Q2)
        {
            return Q1.x * Q2.x + Q1.y * Q2.y + Q1.z * Q2.z + Q1.w * Q2.w;
        }

        inline f32 Dot(const Quat& Other)
        {
            return x * Other.x + y * Other.y + z * Other.z + w * Other.w;
        }

        inline f32 LengthSquared()
        {
            return x * x + y * y + z * z + w * w;
        }

        inline const f32 LengthSquared() const
        {
            return x * x + y * y + z * z + w * w;
        }

        inline f32 Length()
        {
            f32 lensq = x * x + y * y + z * z + w * w;
            return sqrtf(lensq);
        }

        inline const f32 Length() const
        {
            f32 lensq = x * x + y * y + z * z + w * w;
            return sqrtf(lensq);
        }

        void Normalize();

        Quat GetNormal();

        static Quat GetNormal(const Quat& Q);

        inline void Conjugate()
        {
            x = -x;
            y = -y;
            z = -z;
        }

        inline Quat GetConjugate()
        {
            return Quat(
                -x,
                -y,
                -z,
                w
            );
        }

        static Quat Inverse(const Quat& Q);

        Quat GetInverse();

        const Quat GetInverse() const;

        Quat GetNeighborhood(const Quat& Other);

        static Quat Mix(const Quat& From, const Quat& To, f32 T)
        {
            return From * (1.0f -T) + To * T;
        }

        static Quat NLerp(const Quat& From, const Quat& To, f32 T)
        {
            return Quat::GetNormal(From + (To - From) * T);
        }

        static Quat SLerp(const Quat& Start, const Quat& End, f32 T)
        {
            if(fabsf(Quat::Dot(Start, End)) > 1.0f - EPSILON)
            {
                return Quat::NLerp(Start, End, T);
            }

            Quat delta = Start.GetInverse() * End;
            return Quat::GetNormal((delta^T) * Start);
        }

        static Quat LookRotation(const Vec3& Direction, const Vec3& Up, const Vec3& Right);

        Mat4 ToMat4();

        static Mat4 ToMat4(const Quat& Q);

    private:


    };
}