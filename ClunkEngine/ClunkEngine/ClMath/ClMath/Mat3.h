#pragma once


#include <PCH/pch.h>

#include "Vec3.h"
#include <cmath>

namespace Clunk
{
    class Mat3
    {
    public:

        union{

            f32 Data[9];

            struct 
            {
                Vec3 Right;
                Vec3 Up;
                Vec3 Forward;
            };

            struct
            {
                f32 xx, xy, xz;
                f32 yx, yy, yz;
                f32 zx, zy, zz;
            };
            
            struct
            {
                f32 r0c0, r1c0, r2c0;
                f32 r0c1, r1c1, r2c1;
                f32 r0c2, r1c2, r2c2;
            };
            
            f32 Row[3][3];
            
        };
    
        Mat3(
                f32 Mat00 = 0.0f, f32 Mat01 = 0.0f, f32 Mat02 = 0.0f,
                f32 Mat10 = 0.0f, f32 Mat11 = 0.0f, f32 Mat12 = 0.0f,
                f32 Mat20 = 0.0f, f32 Mat21 = 0.0f, f32 Mat22 = 0.0f
            )
        {
            Row[0][0] = Mat00; Row[0][1] = Mat01; Row[0][2] = Mat02;
            Row[1][0] = Mat10; Row[1][1] = Mat11; Row[1][2] = Mat12;
            Row[2][0] = Mat20; Row[2][1] = Mat21; Row[2][2] = Mat22;
        }

        Mat3(const Vec3& A, const Vec3& B, const Vec3& C)
        {
            Row[0][0] = A.X; Row[0][1] = A.Y; Row[0][2] = A.Z;
            Row[1][0] = B.X; Row[1][1] = B.Y; Row[1][2] = B.Z;
            Row[2][0] = C.X; Row[2][1] = C.Y; Row[2][2] = C.Z;
        }

        Vec3 RowVec(u8 I)
        {
            // return (*reinterpret_cast<Vec3*>(Row[I]));
            return Vec3(Row[I][0], Row[I][1], Row[I][2]);
        }

        Vec3 ColVec(u8 J)
        {
            return Vec3(Row[0][J], Row[1][J], Row[2][J]);
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
            return (Row[I][J]);
        }

        const float& operator()(u8 I, u8 J) const
        {
            return (Row[I][J]);
        }

        Vec3& operator[](u8 I)
        {
            return(*reinterpret_cast<Vec3*>(Row[I]));
        }

        const Vec3& operator[](u8 J) const
        {
            return(*reinterpret_cast<const Vec3*>(Row[J]));
        }

/////////////////////////////////////////////////////////////////////

        // void operator=(Mat3& Other)
        // {
        //     Row = Other.Row;
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
                (*this)(0, 0) * Val.X + (*this)(0, 1) * Val.Y + (*this)(0, 2) * Val.Z,
                (*this)(1, 0) * Val.X + (*this)(1, 1) * Val.Y + (*this)(1, 2) * Val.Z,
                (*this)(2, 0) * Val.X + (*this)(2, 1) * Val.Y + (*this)(2, 2) * Val.Z
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

            f32 x = Axis.X * d;
            f32 y = Axis.Y * d;
            f32 z = Axis.Z * d;
            f32 axay = x * Axis.Y;
            f32 axaz = x * Axis.Z;
            f32 ayaz = y * Axis.Z;

            Mat3 m(
                c + x * Axis.X, axay - s * Axis.Z, axaz + s * Axis.Y,
                axay + s * Axis.Z, c + y * Axis.Y, ayaz - s * Axis.X,
                axaz - s * Axis.Y, ayaz + s * Axis.X, c + z * Axis.Z
            );
            return (*this) * m;
        }


        
    };
}