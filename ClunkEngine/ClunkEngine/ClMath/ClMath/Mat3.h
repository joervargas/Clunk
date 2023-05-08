#pragma once


#include <PCH/pch.h>

#include "Vec3.h"
#include <cmath>

namespace Clunk
{
    class Mat3
    {
    public:

        f32 mat[3][3];
    
        Mat3(
                f32 Mat00 = 0.0f, f32 Mat01 = 0.0f, f32 Mat02 = 0.0f,
                f32 Mat10 = 0.0f, f32 Mat11 = 0.0f, f32 Mat12 = 0.0f,
                f32 Mat20 = 0.0f, f32 Mat21 = 0.0f, f32 Mat22 = 0.0f
            )
        {
            mat[0][0] = Mat00; mat[0][1] = Mat01; mat[0][2] = Mat02;
            mat[1][0] = Mat10; mat[1][1] = Mat11; mat[1][2] = Mat12;
            mat[2][0] = Mat20; mat[2][1] = Mat21; mat[2][2] = Mat22;
        }

        Mat3(const Vec3& A, const Vec3& B, const Vec3& C)
        {
            mat[0][0] = A.x; mat[0][1] = A.y; mat[0][2] = A.z;
            mat[1][0] = B.x; mat[1][1] = B.y; mat[1][2] = B.z;
            mat[2][0] = C.x; mat[2][1] = C.y; mat[2][2] = C.z;
        }

        Vec3 Row(u8 I)
        {
            return(*reinterpret_cast<Vec3*>(mat[I]));
        }

        Vec3 Col(u8 J)
        {
            return Vec3(mat[0][J], mat[1][J], mat[2][J]);
        }

        static Mat3 Identity()
        {
            return Mat3(
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }

        static Mat3 Zero()
        {
            return Mat3(
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f
            );
        }

        float& operator()(u8 I, u8 J)
        {
            return (mat[I][J]);
        }

        const float& operator()(u8 I, u8 J) const
        {
            return (mat[I][J]);
        }

        Vec3& operator[](u8 I)
        {
            return(*reinterpret_cast<Vec3*>(mat[I]));
        }

        const Vec3& operator[](u8 J) const
        {
            return(*reinterpret_cast<const Vec3*>(mat[J]));
        }

/////////////////////////////////////////////////////////////////////

        // void operator=(Mat3& Other)
        // {
        //     mat = Other.mat;
        // }
        
/////////////////////////////////////////////////////////////////////

        Mat3 operator*(const Mat3& Other)
        {
            return Mat3(
                (*this)(0, 0) * Other(0, 0) + (*this)(0, 1) * Other(1, 0) + (*this)(0, 2) * Other(2, 0),
                (*this)(0, 0) * Other(0, 1) + (*this)(0, 1) * Other(1, 1) + (*this)(0, 2) * Other(2, 1),
                (*this)(0, 0) * Other(0, 2) + (*this)(0, 1) * Other(1, 2) + (*this)(0, 2) * Other(2, 2),

                (*this)(1, 0) * Other(0, 0) + (*this)(1, 1) * Other(1, 0) + (*this)(1, 2) * Other(2, 0),
                (*this)(1, 0) * Other(0, 1) + (*this)(1, 1) * Other(1, 1) + (*this)(1, 2) * Other(2, 1),
                (*this)(1, 0) * Other(0, 2) + (*this)(1, 1) * Other(1, 2) + (*this)(1, 2) * Other(2, 2),

                (*this)(2, 0) * Other(0, 0) + (*this)(2, 1) * Other(1, 0) + (*this)(2, 2) * Other(2, 0),
                (*this)(2, 0) * Other(0, 1) + (*this)(2, 1) * Other(1, 1) + (*this)(2, 2) * Other(2, 1),
                (*this)(2, 0) * Other(0, 2) + (*this)(2, 1) * Other(1, 2) + (*this)(2, 2) * Other(2, 2)
            );
        }

        // Mat3 operator*(const Mat3& other)
        // {
        //     return Mat3(
        //         (*this)(0, 0) * other(0, 0) + (*this)(0, 1) * other(1, 0) + (*this)(0, 2) * other(2, 0),
        //         (*this)(0, 0) * other(0, 1) + (*this)(0, 1) * other(1, 1) + (*this)(0, 2) * other(2, 1),
        //         (*this)(0, 0) * other(0, 2) + (*this)(0, 1) * other(1, 2) + (*this)(0, 2) * other(2, 2),
        //         (*this)(1, 0) * other(0, 0) + (*this)(1, 1) * other(1, 0) + (*this)(1, 2) * other(2, 0),
        //         (*this)(1, 0) * other(0, 1) + (*this)(1, 1) * other(1, 1) + (*this)(1, 2) * other(2, 1),
        //         (*this)(1, 0) * other(0, 2) + (*this)(1, 1) * other(1, 2) + (*this)(1, 2) * other(2, 1),
        //         (*this)(2, 0) * other(0, 0) + (*this)(2, 1) * other(1, 0) + (*this)(2, 2) * other(2, 0),
        //         (*this)(2, 0) * other(0, 1) + (*this)(2, 1) * other(1, 1) + (*this)(2, 2) * other(2, 1),
        //         (*this)(2, 0) * other(0, 2) + (*this)(2, 1) * other(1, 2) + (*this)(2, 2) * other(2, 2)
        //     );
        // }

        Vec3 operator*(const Vec3& Val)
        {
            Vec3 v(
                (*this)(0, 0) * Val.x + (*this)(0, 1) * Val.y + (*this)(0, 2) * Val.z,
                (*this)(1, 0) * Val.x + (*this)(1, 1) * Val.y + (*this)(1, 2) * Val.z,
                (*this)(2, 0) * Val.x + (*this)(2, 1) * Val.y + (*this)(2, 2) * Val.z
            );
            return v;
        }


////////////////////////////////////////////////////

        Mat3 RotateX(f32 Angle)
        {
            f32 c = std::cos(Angle);
            f32 s = std::sin(Angle);

            Mat3 m(
                1.0f, 0.0f, 0.0f,
                0.0f, c, -s,
                0.0f, s, c
            );
            return (*this) * m;
        }

        Mat3 RotateY(f32 Angle)
        {
            f32 c = std::cos(Angle);
            f32 s = std::sin(Angle);

            Mat3 m(
                c, 0.0f, s,
                0.0f, 1.0f, 0.0f,
                -s, 0.0f, c
            );
            return (*this) * m;
        }

        Mat3 RotateZ(f32 Angle)
        {
            f32 c = std::cos(Angle);
            f32 s = std::sin(Angle);

            Mat3 m(
                c, -s, 0.0f,
                s, c, 0.0f,
                0.0f, 0.0f, 1.0f
            );
            return (*this) * m;
        }

        Mat3 Rotate(f32 T, const Vec3& Axis)
        {
            f32 c = std::cos(T);
            f32 s = std::sin(T);
            f32 d = 1.0f - c;

            f32 x = Axis.x * d;
            f32 y = Axis.y * d;
            f32 z = Axis.z * d;
            f32 axay = x * Axis.y;
            f32 axaz = x * Axis.z;
            f32 ayaz = y * Axis.z;

            Mat3 m(
                c + x * Axis.x, axay - s * Axis.z, axaz + s * Axis.y,
                axay + s * Axis.z, c + y * Axis.y, ayaz - s * Axis.x,
                axaz - s * Axis.y, ayaz + s * Axis.x, c + z * Axis.z
            );
            return (*this) * m;
        }


        
    };
}