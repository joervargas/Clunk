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
                f32 X, Y , Z, W;
            };
            struct
            {
                Vec3 Vec;
                f32 Scalar;
            };
            f32 q[4];
        };

        Quat(f32 _x = 0.0f, f32 _y = 0.0f, f32 _z = 0.0f, f32 _w = 1.0f) : 
            X(_x), Y(_y), Z(_z), W(_w) {}

        Quat(const Vec3& V, f32 S = 1.0f)
        {
            X = V.X; Y = V.Y; Z = V.Z; W = S;
        }

        void operator=(const Quat& Other)
        {
            X = Other.X;
            Y = Other.Y;
            Z = Other.Z;
            W = Other.W;
        }

        bool operator==(const Quat& Other) const
        {
            return (
                fabsf(X - Other.X) <= EPSILON &&
                fabsf(Y - Other.Y) <= EPSILON &&
                fabsf(Z - Other.Z) <= EPSILON &&
                fabsf(W - Other.W) <= EPSILON
            );
        }

        bool operator!=(const Quat& Other)
        {
            return !((*this) == Other);
        }

        Quat operator+(const Quat& Other)
        {
            return Quat(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
        }

        const Quat operator+(const Quat& Other) const
        {
            return Quat(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
        }

        Quat operator-(const Quat& Other)
        {
            return Quat(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
        }

        const Quat operator-(const Quat& Other) const
        {
            return Quat(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
        }

        Quat operator-()
        {
            return Quat(-X, -Y, -Z, -W);
        }

        const Quat operator-() const
        {
            return Quat(-X, -Y, -Z, -W);
        }

        Quat operator*(f32 Val)
        {
            return Quat(X * Val, Y * Val, Z * Val, W * Val);
        }

        const Quat operator*(f32 Val) const
        {
            return Quat(X * Val, Y * Val, Z * Val, W * Val);
        }

        Quat operator*(const Quat& Other)
        {
            Quat result;
            result.Scalar = Other.Scalar * Scalar - Vec3::Dot(Other.Vec, Vec);
            result.Vec = (Vec * Other.Scalar) + (Other.Vec * Scalar) + Vec3::Cross(Other.Vec, Vec);
            return result;
        }

        const Quat operator*(const Quat& Other) const
        {
            Quat result;
            result.Scalar = Other.Scalar * Scalar - Vec3::Dot(Other.Vec, Vec);
            result.Vec = (Vec * Other.Scalar) + (Other.Vec * Scalar) + Vec3::Cross(Other.Vec, Vec);
            return result;
        }

        Vec3 operator*(const Vec3& V)
        {
            return Vec * 2.0f * Vec3::Dot(Vec, V) + 
                V * (Scalar * Scalar - Vec3::Dot(Vec, Vec)) + 
                Vec3::Cross(Vec, V) * 2.0f * Scalar;
        }

        const Vec3 operator*(const Vec3& V) const
        {
            return Vec * 2.0f * Vec3::Dot(Vec, V) + 
                V * (Scalar * Scalar - Vec3::Dot(Vec, Vec)) + 
                Vec3::Cross(Vec, V) * 2.0f * Scalar;
        }

        Quat operator^(f32 Val)
        {
            f32 angle = 2.0f * acosf(Scalar);
            Vec3 axis = Vec3::Normalize(Vec);

            f32 halfCos = cosf(Val * angle * 0.5f);
            f32 halfSin = sinf(Val * angle * 0.5f);

            return Quat(
                axis.X * halfSin,
                axis.Y * halfSin,
                axis.Z * halfSin,
                halfCos
            );
        }

        static Quat Identity()
        {
            return Quat(0.0f, 0.0f, 0.0f, 1.0f);
        }

        void SetAngleAxis(f32 Degrees, const Vec3& Axis);

        static Quat AngleAxis(f32 Degrees, const Vec3& Axis);

        static Quat FromTo(const Vec3& From, const Vec3& To);

        static inline Vec3 GetAxis(const Quat& Q)
        {
            return Vec3::Normalize(Vec3(Q.X, Q.Y, Q.Z));
        }

        inline Vec3 GetAxis()
        {
            return Vec3::Normalize(Vec3(X, Y, Z));
        }

        static inline f32 GetAngle(const Quat& Q)
        {
            return 2.0f * acosf(Q.W);
        }

        inline f32 GetAngle()
        {
            return 2.0f * acosf(W);
        }

        static inline f32 Dot(const Quat& Q1, const Quat& Q2)
        {
            return Q1.X * Q2.X + Q1.Y * Q2.Y + Q1.Z * Q2.Z + Q1.W * Q2.W;
        }

        inline f32 Dot(const Quat& Other)
        {
            return X * Other.X + Y * Other.Y + Z * Other.Z + W * Other.W;
        }

        inline f32 LengthSquared()
        {
            return X * X + Y * Y + Z * Z + W * W;
        }

        inline const f32 LengthSquared() const
        {
            return X * X + Y * Y + Z * Z + W * W;
        }

        inline f32 Length()
        {
            f32 lensq = X * X + Y * Y + Z * Z + W * W;
            return sqrtf(lensq);
        }

        inline const f32 Length() const
        {
            f32 lensq = X * X + Y * Y + Z * Z + W * W;
            return sqrtf(lensq);
        }

        void Normalize();

        Quat GetNormal();

        static Quat GetNormal(const Quat& Q);

        inline void Conjugate()
        {
            X = -X;
            Y = -Y;
            Z = -Z;
        }

        inline Quat GetConjugate()
        {
            return Quat(
                -X,
                -Y,
                -Z,
                W
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

        static Quat LookRotation(const Vec3& Direction, Vec3& Up);

        void LookRotaion(const Vec3& Direction, Vec3& Up);

        Mat4 ToMat4();

        Mat4 ToMat4() const;

        static Mat4 ToMat4(const Quat& Q);

    private:


    };
}
