#pragma once


#include <PCH/pch.h>

#include "Vec3.h"
#include "Vec4.h"
// #include "Quat.h"
#include <cmath>

namespace Clunk
{
    // Forward declarations
    class Quat;
    class Transform3d;

    // Column Major
    class Mat4
    {
    public:

        union{

            f32 Data[16];

            struct 
            {
                Vec4 Right;
                Vec4 Up;
                Vec4 Forward;
                Vec4 Position;
            };

            struct
            {
                f32 xx, xy, xz, xw;
                f32 yx, yy, yz, yw;
                f32 zx, zy, zz, zw;
                f32 tx, ty, tz, tw;
            };
            
            struct
            {
                f32 r0c0, r1c0, r2c0, r3c0;
                f32 r0c1, r1c1, r2c1, r3c1;
                f32 r0c2, r1c2, r2c2, r3c2;
                f32 r0c3, r1c3, r2c3, r3c3;
            };

            f32 Row[4][4];
            
        };


        inline Mat4(
                f32 Mat0 = 0.0f, f32 Mat1 = 0.0f, f32 Mat2 = 0.0f, f32 Mat3 = 0.0f,
                f32 Mat4 = 0.0f, f32 Mat5 = 0.0f, f32 Mat6 = 0.0f, f32 Mat7 = 0.0f,
                f32 Mat8 = 0.0f, f32 Mat9 = 0.0f, f32 Mat10 = 0.0f, f32 Mat11 = 0.0f,
                f32 Mat12 = 0.0f, f32 Mat13 = 0.0f, f32 Mat14 = 0.0f, f32 Mat15 = 0.0f
            )
        {
            Data[0] = Mat0;   Data[1] = Mat1;   Data[2] = Mat2;   Data[3] = Mat3;
            Data[4] = Mat4;   Data[5] = Mat5;   Data[6] = Mat6;   Data[7] = Mat7;
            Data[8] = Mat8;   Data[9] = Mat9;   Data[10] = Mat10; Data[11] = Mat11;
            Data[12] = Mat12; Data[13] = Mat13; Data[14] = Mat14; Data[15] = Mat15;
        }

        inline Mat4(f32 m[16])
        {
            xx = m[0];  xy = m[1];  xz = m[2];  xw = m[3];
            yx = m[4];  yy = m[5];  yz = m[6];  yw = m[7];
            zx = m[8];  zy = m[9];  zz = m[10]; zw = m[11];
            tx = m[12]; ty = m[13]; tz = m[14]; tw = m[15];
        }

        static Mat4 Identity()
        {
            return Mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        static Mat4 Zero()
        {
            return Mat4(
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f
            );
        }

        Vec4 RowVec(u8 I)
        {
            // return (*reinterpret_cast<Vec4*>(Row[I]));
            return Vec4(Row[I][0], Row[I][1], Row[I][2], Row[I][3]);
        }

        Vec4 ColVec(u8 J)
        {
            return Vec4(Row[0][J], Row[1][J], Row[2][J], Row[3][J]);
        }

        void operator=(const Mat4& Other)
        {
            for(i32 i = 0; i < 16; ++i)
            {
                Data[i] = Other[i];
            }
        }

        bool operator==(const Mat4& Other)
        {
            for(i32 i = 0; i < 16; ++i)
            {
                if(fabs(Data[i] - Other.Data[i]) > EPSILON)
                {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Mat4& Other)
        {
            return !((*this) == Other);
        }

        f32& operator[](u8 I)
        {
            return Data[I];
        }

        const f32& operator[](u8 I) const
        {
            return Data[I];
        }

    ////////////////////////////////////////////////////////////

       Mat4 operator+(const Mat4& Other)
        {
            return Mat4(
                Data[0]  + Other.Data[0], 
                Data[1]  + Other.Data[1], 
                Data[2]  + Other.Data[2], 
                Data[3]  + Other.Data[3], 

                Data[4]  + Other.Data[4], 
                Data[5]  + Other.Data[5], 
                Data[6]  + Other.Data[6], 
                Data[7]  + Other.Data[7], 

                Data[8]  + Other.Data[8], 
                Data[9]  + Other.Data[9], 
                Data[10] + Other.Data[10],
                Data[11] + Other.Data[11], 

                Data[12] + Other.Data[12], 
                Data[13] + Other.Data[13], 
                Data[14] + Other.Data[14], 
                Data[15] + Other.Data[15]
            );
        }

        Mat4 operator+(const f32 Val)
        {
            return Mat4(
                Data[0]  + Val, 
                Data[1]  + Val, 
                Data[2]  + Val, 
                Data[3]  + Val, 

                Data[4]  + Val, 
                Data[5]  + Val, 
                Data[6]  + Val, 
                Data[7]  + Val, 

                Data[8]  + Val, 
                Data[9]  + Val, 
                Data[10] + Val,
                Data[11] + Val, 

                Data[12] + Val, 
                Data[13] + Val, 
                Data[14] + Val, 
                Data[15] + Val
            );
        }

/////////////////////////////////////////////////

        Mat4 operator-(const Mat4& Other)
        {
            return Mat4(
                Data[0]  - Other[0], 
                Data[1]  - Other[1], 
                Data[2]  - Other[2], 
                Data[3]  - Other[3], 

                Data[4]  - Other[4], 
                Data[5]  - Other[5], 
                Data[6]  - Other[6], 
                Data[7]  - Other[7], 

                Data[8]  - Other[8], 
                Data[9]  - Other[9], 
                Data[10] - Other[10],
                Data[11] - Other[11], 

                Data[12] - Other[12], 
                Data[13] - Other[13], 
                Data[14] - Other[14], 
                Data[15] - Other[15]
            );
        }

        Mat4 operator-(const f32 Val)
        {
            return Mat4(
                Data[0]  - Val, 
                Data[1]  - Val, 
                Data[2]  - Val, 
                Data[3]  - Val, 

                Data[4]  - Val, 
                Data[5]  - Val, 
                Data[6]  - Val, 
                Data[7]  - Val, 

                Data[8]  - Val, 
                Data[9]  - Val, 
                Data[10] - Val,
                Data[11] - Val, 

                Data[12] - Val, 
                Data[13] - Val, 
                Data[14] - Val, 
                Data[15] - Val
            );
        }
/////////////////////////////////////////////////

        #define M4D(aRow, bCol) \
            Data[0 * 4 + aRow] * Other.Data[bCol * 4 + 0] + \
            Data[1 * 4 + aRow] * Other.Data[bCol * 4 + 1] + \
            Data[2 * 4 + aRow] * Other.Data[bCol * 4 + 2] + \
            Data[3 * 4 + aRow] * Other.Data[bCol * 4 + 3] 

        Mat4 operator*(const Mat4& Other)
        {
            Mat4 result;
            result[0]  = (Data[0]  * Other[0]) + (Data[1]  * Other[4]) + (Data[2]  * Other[8])  + (Data[3] * Other[12]);
            result[1]  = (Data[0]  * Other[1]) + (Data[1]  * Other[5]) + (Data[2]  * Other[9])  + (Data[3] * Other[13]);
            result[2]  = (Data[0]  * Other[2]) + (Data[1]  * Other[6]) + (Data[2]  * Other[10]) + (Data[3] * Other[14]);
            result[3]  = (Data[0]  * Other[3]) + (Data[1]  * Other[7]) + (Data[2]  * Other[11]) + (Data[3] * Other[15]);

            result[4]  = (Data[4]  * Other[0]) + (Data[5]  * Other[4]) + (Data[6]  * Other[8])  + (Data[7] * Other[12]);
            result[5]  = (Data[4]  * Other[1]) + (Data[5]  * Other[5]) + (Data[6]  * Other[9])  + (Data[7] * Other[13]);
            result[6]  = (Data[4]  * Other[2]) + (Data[5]  * Other[6]) + (Data[6]  * Other[10]) + (Data[7] * Other[14]);
            result[7]  = (Data[4]  * Other[3]) + (Data[5]  * Other[7]) + (Data[6]  * Other[11]) + (Data[7] * Other[15]);

            result[8]  = (Data[8]  * Other[0]) + (Data[9]  * Other[4]) + (Data[10] * Other[8])  + (Data[11] * Other[12]);
            result[9]  = (Data[8]  * Other[1]) + (Data[9]  * Other[5]) + (Data[10] * Other[9])  + (Data[11] * Other[13]);
            result[10] = (Data[8]  * Other[2]) + (Data[9]  * Other[6]) + (Data[10] * Other[10]) + (Data[11] * Other[14]);
            result[11] = (Data[8]  * Other[3]) + (Data[9]  * Other[7]) + (Data[10] * Other[11]) + (Data[11] * Other[15]);

            result[12] = (Data[12] * Other[0]) + (Data[13] * Other[4]) + (Data[14] * Other[8])  + (Data[15] * Other[12]);
            result[13] = (Data[12] * Other[1]) + (Data[13] * Other[5]) + (Data[14] * Other[9])  + (Data[15] * Other[13]);
            result[14] = (Data[12] * Other[2]) + (Data[13] * Other[6]) + (Data[14] * Other[10]) + (Data[15] * Other[14]);
            result[15] = (Data[12] * Other[3]) + (Data[13] * Other[7]) + (Data[14] * Other[11]) + (Data[15] * Other[15]);
            return result;
        }

        const Mat4 operator*(const Mat4& Other) const
        {
            Mat4 result;
            result[0]  = (Data[0] * Other[0])  + (Data[1]  * Other[4]) + (Data[2]  * Other[8])  + (Data[3] * Other[12]);
            result[1]  = (Data[0] * Other[1])  + (Data[1]  * Other[5]) + (Data[2]  * Other[9])  + (Data[3] * Other[13]);
            result[2]  = (Data[0] * Other[2])  + (Data[1]  * Other[6]) + (Data[2]  * Other[10]) + (Data[3] * Other[14]);
            result[3]  = (Data[0] * Other[3])  + (Data[1]  * Other[7]) + (Data[2]  * Other[11]) + (Data[3] * Other[15]);

            result[4]  = (Data[4] * Other[0])  + (Data[5]  * Other[4]) + (Data[6]  * Other[8])  + (Data[7] * Other[12]);
            result[5]  = (Data[4] * Other[1])  + (Data[5]  * Other[5]) + (Data[6]  * Other[9])  + (Data[7] * Other[13]);
            result[6]  = (Data[4] * Other[2])  + (Data[5]  * Other[6]) + (Data[6]  * Other[10]) + (Data[7] * Other[14]);
            result[7]  = (Data[4] * Other[3])  + (Data[5]  * Other[7]) + (Data[6]  * Other[11]) + (Data[7] * Other[15]);

            result[8]  = (Data[8]  * Other[0]) + (Data[9]  * Other[4]) + (Data[10] * Other[8])  + (Data[11] * Other[12]);
            result[9]  = (Data[8]  * Other[1]) + (Data[9]  * Other[5]) + (Data[10] * Other[9])  + (Data[11] * Other[13]);
            result[10] = (Data[8]  * Other[2]) + (Data[9]  * Other[6]) + (Data[10] * Other[10]) + (Data[11] * Other[14]);
            result[11] = (Data[8]  * Other[3]) + (Data[9]  * Other[7]) + (Data[10] * Other[11]) + (Data[11] * Other[15]);

            result[12] = (Data[12] * Other[0]) + (Data[13] * Other[4]) + (Data[14] * Other[8])  + (Data[15] * Other[12]);
            result[13] = (Data[12] * Other[1]) + (Data[13] * Other[5]) + (Data[14] * Other[9])  + (Data[15] * Other[13]);
            result[14] = (Data[12] * Other[2]) + (Data[13] * Other[6]) + (Data[14] * Other[10]) + (Data[15] * Other[14]);
            result[15] = (Data[12] * Other[3]) + (Data[13] * Other[7]) + (Data[14] * Other[11]) + (Data[15] * Other[15]);
            return result;
        }


        Vec4 operator*(const Vec4& Vec)
        {
            return Vec4(
                (Data[0] * Vec.X) + (Data[4] * Vec.Y) + (Data[8]  * Vec.Z) + (Data[12] * Vec.W),
                (Data[1] * Vec.X) + (Data[5] * Vec.Y) + (Data[9]  * Vec.Z) + (Data[13] * Vec.W),
                (Data[2] * Vec.X) + (Data[6] * Vec.Y) + (Data[10] * Vec.Z) + (Data[14] * Vec.W),
                (Data[3] * Vec.X) + (Data[7] * Vec.Y) + (Data[11] * Vec.Z) + (Data[15] * Vec.W)
            );
        }

        Mat4 operator*(const f32 Val)
        {
            return Mat4(
                xx * Val, xy * Val, xz * Val, xw * Val,
                yx * Val, yy * Val, yz * Val, yw * Val,
                zx * Val, zy * Val, zz * Val, zw * Val,
                tx * Val, ty * Val, tz * Val, tw * Val
            );
        }

////////////////////////////////////////////////////

        void RotateX(f32 Angle);

        static Mat4 RotateX(const Mat4& Mat, f32 Angle);

        void RotateY(f32 Angle);

        static Mat4 RotateY(const Mat4& Mat, f32 Angle);

        void RotateZ(f32 Angle);

        static Mat4 RotateZ(const Mat4& Mat, f32 Angle);

        static Mat4 Scale(const Mat4& Mat, const Vec3& Scale);

        void Scale(const Vec3& Scale);

        static Mat4 Scale(const Mat4& Mat, const f32 Scale);

        void Scale(const f32 Scale);

        static Mat4 Translate(const Mat4& Mat, const Vec3& Translation);

        void Translate(const Vec3& Translation);

        static Mat4 ReflectionMatrix(const Vec3& Axis);

        /**
         * @brief Look At Right Hand
         * @param Eye Vec3 Position
         * @param Center Vec3 target to look at
         * @param Up Vec3 representing the up axis
         * @return Mat4 
         */
        static Mat4 LookAtRH(Vec3 const& Eye, Vec3 const& Center, Vec3 const& Up);
        static Mat4 LookAtLH(Vec3 const& Eye, Vec3 const& Center, Vec3 const& Up);

        /**
         * @brief Perspective Projection Matrix
         * @param FOV Field of View angle
         * @param ApectRatio Aspect-Ratio of the drawable area. (height / width) or (width / height)
         * @param Near Near depth plane
         * @param Far Far depth plane
         * @return Mat4 
         */
        static Mat4 PerspectiveRH(f32 FOV, f32 ApectRatio, f32 Near, f32 Far);      
        static Mat4 PerspectiveLH(f32 FOV, f32 AspectRatio, f32 Near, f32 Far);  
        static Mat4 PerspectiveVK(f32 FOV, f32 AspectRatio, f32 Near, f32 Far);

        static Mat4 OrthographicRH(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far);
        static Mat4 OrthographicLH(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far);
        static Mat4 OrthographicVK(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far);

        Mat4 GetTranspose()
        {
            return Mat4(
                xx, yx, zx, tx,
                xy, yy, zy, ty,
                xz, yz, zz, tz,
                xw, yw, zw, tw
            );
        }

        static Mat4 Transpose(const Mat4& Mat)
        {
            return Mat4(
                Mat.xx, Mat.yx, Mat.zx, Mat.tx,
                Mat.xy, Mat.yy, Mat.zy, Mat.ty,
                Mat.xz, Mat.yz, Mat.zz, Mat.tz,
                Mat.xw, Mat.yw, Mat.zw, Mat.tw
            );
        }

        static Mat4 Inverse(const Mat4& Mat);

        void Invert();

        static Quat ToQuat(const Mat4& Mat);

        Quat ToQuat();

        const Quat ToQuat() const;

        static Transform3d ToTransform3d(const Mat4& Mat);

        Transform3d ToTransform3d();

        const Transform3d ToTransform3d() const;

        static Mat4 FromTransform3d(const Transform3d& T);

    private:

    };

}