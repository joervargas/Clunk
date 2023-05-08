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

            f32 mat[16];

            struct 
            {
                Vec4 right;
                Vec4 up;
                Vec4 forward;
                Vec4 position;
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
            
        };


        inline Mat4(
                f32 Mat0 = 0.0f, f32 Mat1 = 0.0f, f32 Mat2 = 0.0f, f32 Mat3 = 0.0f,
                f32 Mat4 = 0.0f, f32 Mat5 = 0.0f, f32 Mat6 = 0.0f, f32 Mat7 = 0.0f,
                f32 Mat8 = 0.0f, f32 Mat9 = 0.0f, f32 Mat10 = 0.0f, f32 Mat11 = 0.0f,
                f32 Mat12 = 0.0f, f32 Mat13 = 0.0f, f32 Mat14 = 0.0f, f32 Mat15 = 0.0f
            )
        {
            mat[0] = Mat0; mat[1] = Mat1; mat[2] = Mat2; mat[3] = Mat3;
            mat[4] = Mat4; mat[5] = Mat5; mat[6] = Mat6; mat[7] = Mat7;
            mat[8] = Mat8; mat[9] = Mat9; mat[10] = Mat10; mat[11] = Mat11;
            mat[12] = Mat12; mat[13] = Mat13; mat[14] = Mat14; mat[15] = Mat15;
        }

        inline Mat4(f32 m[16])
        {
            xx = m[0]; xy = m[1]; xz = m[2]; xw = m[3];
            yx = m[4]; yy = m[5]; yz = m[6]; yw = m[7];
            zx = m[8]; zy = m[9]; zz = m[10]; zw = m[11];
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

        void operator=(const Mat4& Other)
        {
            for(i32 i = 0; i < 16; ++i)
            {
                mat[i] = Other[i];
            }
        }

        bool operator==(const Mat4& Other)
        {
            for(i32 i = 0; i < 16; ++i)
            {
                if(fabs(mat[i] - Other.mat[i]) > EPSILON)
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
            return mat[I];
        }

        const f32& operator[](u8 I) const
        {
            return mat[I];
        }

    ////////////////////////////////////////////////////////////

       Mat4 operator+(const Mat4& Other)
        {
            return Mat4(
                mat[0] + Other[0], 
                mat[1] + Other[1], 
                mat[2] + Other[2], 
                mat[3] + Other[3], 

                mat[4] + Other[4], 
                mat[5] + Other[5], 
                mat[6] + Other.mat[6], 
                mat[7] + Other.mat[7], 

                mat[8] + Other.mat[8], 
                mat[9] + Other.mat[9], 
                mat[10] + Other.mat[10],
                mat[11] + Other.mat[11], 

                mat[12] + Other.mat[12], 
                mat[13] + Other.mat[13], 
                mat[14] + Other.mat[14], 
                mat[15] + Other.mat[15]
            );
        }

        Mat4 operator+(const f32 Val)
        {
            return Mat4(
                mat[0] + Val, 
                mat[1] + Val, 
                mat[2] + Val, 
                mat[3] + Val, 

                mat[4] + Val, 
                mat[5] + Val, 
                mat[6] + Val, 
                mat[7] + Val, 

                mat[8] + Val, 
                mat[9] + Val, 
                mat[10] + Val,
                mat[11] + Val, 

                mat[12] + Val, 
                mat[13] + Val, 
                mat[14] + Val, 
                mat[15] + Val
            );
        }

/////////////////////////////////////////////////

        Mat4 operator-(const Mat4& Other)
        {
            return Mat4(
                mat[0] - Other[0], 
                mat[1] - Other[1], 
                mat[2] - Other[2], 
                mat[3] - Other[3], 

                mat[4] - Other[4], 
                mat[5] - Other[5], 
                mat[6] - Other[6], 
                mat[7] - Other[7], 

                mat[8] - Other[8], 
                mat[9] - Other[9], 
                mat[10] - Other[10],
                mat[11] - Other[11], 

                mat[12] - Other[12], 
                mat[13] - Other[13], 
                mat[14] - Other[14], 
                mat[15] - Other[15]
            );
        }

        Mat4 operator-(const f32 Val)
        {
            return Mat4(
                mat[0] - Val, 
                mat[1] - Val, 
                mat[2] - Val, 
                mat[3] - Val, 

                mat[4] - Val, 
                mat[5] - Val, 
                mat[6] - Val, 
                mat[7] - Val, 

                mat[8] - Val, 
                mat[9] - Val, 
                mat[10] - Val,
                mat[11] - Val, 

                mat[12] - Val, 
                mat[13] - Val, 
                mat[14] - Val, 
                mat[15] - Val
            );
        }
/////////////////////////////////////////////////

        #define M4D(aRow, bCol) \
            mat[0 * 4 + aRow] * Other.mat[bCol * 4 + 0] + \
            mat[1 * 4 + aRow] * Other.mat[bCol * 4 + 1] + \
            mat[2 * 4 + aRow] * Other.mat[bCol * 4 + 2] + \
            mat[3 * 4 + aRow] * Other.mat[bCol * 4 + 3] 

        Mat4 operator*(const Mat4& Other)
        {
            Mat4 result;
            result[0] = (mat[0] * Other[0]) + (mat[1] * Other[4]) + (mat[2] * Other[8]) + (mat[3] * Other[12]);
            result[1] = (mat[0] * Other[1]) + (mat[1] * Other[5]) + (mat[2] * Other[9]) + (mat[3] * Other[13]);
            result[2] = (mat[0] * Other[2]) + (mat[1] * Other[6]) + (mat[2] * Other[10]) + (mat[3] * Other[14]);
            result[3] = (mat[0] * Other[3]) + (mat[1] * Other[7]) + (mat[2] * Other[11]) + (mat[3] * Other[15]);

            result[4] = (mat[4] * Other[0]) + (mat[5] * Other[4]) + (mat[6] * Other[8]) + (mat[7] * Other[12]);
            result[5] = (mat[4] * Other[1]) + (mat[5] * Other[5]) + (mat[6] * Other[9]) + (mat[7] * Other[13]);
            result[6] = (mat[4] * Other[2]) + (mat[5] * Other[6]) + (mat[6] * Other[10]) + (mat[7] * Other[14]);
            result[7] = (mat[4] * Other[3]) + (mat[5] * Other[7]) + (mat[6] * Other[11]) + (mat[7] * Other[15]);

            result[8] = (mat[8] * Other[0]) + (mat[9] * Other[4]) + (mat[10] * Other[8]) + (mat[11] * Other[12]);
            result[9] = (mat[8] * Other[1]) + (mat[9] * Other[5]) + (mat[10] * Other[9]) + (mat[11] * Other[13]);
            result[10] = (mat[8] * Other[2]) + (mat[9] * Other[6]) + (mat[10] * Other[10]) + (mat[11] * Other[14]);
            result[11] = (mat[8] * Other[3]) + (mat[9] * Other[7]) + (mat[10] * Other[11]) + (mat[11] * Other[15]);

            result[12] = (mat[12] * Other[0]) + (mat[13] * Other[4]) + (mat[14] * Other[8]) + (mat[15] * Other[12]);
            result[13] = (mat[12] * Other[1]) + (mat[13] * Other[5]) + (mat[14] * Other[9]) + (mat[15] * Other[13]);
            result[14] = (mat[12] * Other[2]) + (mat[13] * Other[6]) + (mat[14] * Other[10]) + (mat[15] * Other[14]);
            result[15] = (mat[12] * Other[3]) + (mat[13] * Other[7]) + (mat[14] * Other[11]) + (mat[15] * Other[15]);
            return result;
        }

        const Mat4 operator*(const Mat4& Other) const
        {
            Mat4 result;
            result[0] = (mat[0] * Other[0]) + (mat[1] * Other[4]) + (mat[2] * Other[8]) + (mat[3] * Other[12]);
            result[1] = (mat[0] * Other[1]) + (mat[1] * Other[5]) + (mat[2] * Other[9]) + (mat[3] * Other[13]);
            result[2] = (mat[0] * Other[2]) + (mat[1] * Other[6]) + (mat[2] * Other[10]) + (mat[3] * Other[14]);
            result[3] = (mat[0] * Other[3]) + (mat[1] * Other[7]) + (mat[2] * Other[11]) + (mat[3] * Other[15]);

            result[4] = (mat[4] * Other[0]) + (mat[5] * Other[4]) + (mat[6] * Other[8]) + (mat[7] * Other[12]);
            result[5] = (mat[4] * Other[1]) + (mat[5] * Other[5]) + (mat[6] * Other[9]) + (mat[7] * Other[13]);
            result[6] = (mat[4] * Other[2]) + (mat[5] * Other[6]) + (mat[6] * Other[10]) + (mat[7] * Other[14]);
            result[7] = (mat[4] * Other[3]) + (mat[5] * Other[7]) + (mat[6] * Other[11]) + (mat[7] * Other[15]);

            result[8] = (mat[8] * Other[0]) + (mat[9] * Other[4]) + (mat[10] * Other[8]) + (mat[11] * Other[12]);
            result[9] = (mat[8] * Other[1]) + (mat[9] * Other[5]) + (mat[10] * Other[9]) + (mat[11] * Other[13]);
            result[10] = (mat[8] * Other[2]) + (mat[9] * Other[6]) + (mat[10] * Other[10]) + (mat[11] * Other[14]);
            result[11] = (mat[8] * Other[3]) + (mat[9] * Other[7]) + (mat[10] * Other[11]) + (mat[11] * Other[15]);

            result[12] = (mat[12] * Other[0]) + (mat[13] * Other[4]) + (mat[14] * Other[8]) + (mat[15] * Other[12]);
            result[13] = (mat[12] * Other[1]) + (mat[13] * Other[5]) + (mat[14] * Other[9]) + (mat[15] * Other[13]);
            result[14] = (mat[12] * Other[2]) + (mat[13] * Other[6]) + (mat[14] * Other[10]) + (mat[15] * Other[14]);
            result[15] = (mat[12] * Other[3]) + (mat[13] * Other[7]) + (mat[14] * Other[11]) + (mat[15] * Other[15]);
            return result;
        }


        Vec4 operator*(const Vec4& Vec)
        {
            return Vec4(
                (mat[0] * Vec.x) + (mat[4] * Vec.y) + (mat[8] * Vec.z) + (mat[12] * Vec.w),
                (mat[1] * Vec.x) + (mat[5] * Vec.y) + (mat[9] * Vec.z) + (mat[13] * Vec.w),
                (mat[2] * Vec.x) + (mat[6] * Vec.y) + (mat[10] * Vec.z) + (mat[14] * Vec.w),
                (mat[3] * Vec.x) + (mat[7] * Vec.y) + (mat[11] * Vec.z) + (mat[15] * Vec.w)
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