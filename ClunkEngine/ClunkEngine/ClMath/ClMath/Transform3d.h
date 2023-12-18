#pragma once


#include <PCH/pch.h>

#include "Vec3.h"
#include "Quat.h"
#include <cmath>


namespace Clunk
{
    // Forward declarations
    class Mat4;

    class Transform3d
    {
    public:

        struct
        {
            Quat rotation;
            alignas(16) Vec3 position;
            alignas(16) Vec3 scale;
        };

        Transform3d(
            const Vec3& Position = Vec3(0.0f, 0.0f, 0.01f), 
            const Quat& Rotation = Quat(0.0f, 0.0f, 0.0f, 1.0f), 
            const Vec3& Scale = Vec3(1.0f, 1.0f, 1.0f)
        ) : 
            rotation(Rotation), position(Position), scale(Scale)
        {}


        void operator=(const Transform3d& Other)
        {
            position = Other.position;
            rotation = Other.rotation;
            scale = Other.scale;
        }

        // const Transform operator=(const Transform& Other) const
        // {
        //     position = Other.position;
        //     rotation = Other.rotation;
        //     scale = Other.scale;
        // }


        static Transform3d Combine(const Transform3d& A, const Transform3d& B);

        void Combine(const Transform3d& B);

        static Transform3d Inverse(const Transform3d& T);

        Transform3d GetInverse();

        static Transform3d Mix(const Transform3d& A, const Transform3d& B, f32 Time);

        Transform3d Mix(const Transform3d& B, f32 Time);

        static Transform3d FromMat4(const Mat4& Mat);

        static Mat4 ToMat4(const Transform3d& T);

        Mat4 ToMat4();

        const Mat4 ToMat4() const;

    private:


    };
}