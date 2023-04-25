#include "Transform3d.h"
#include "Mat4.h"

namespace Clunk
{

    Transform3d Transform3d::Combine(const Transform3d& A, const Transform3d& B)
    {
        Transform3d result;
    
        result.scale = A.scale * B.scale;
        result.rotation = B.rotation * A.rotation;
    
        result.position = A.rotation * (A.scale * B.position);
        result.position = A.position + result.position;
    
        return result;
    }

    void Transform3d::Combine(const Transform3d& B)
    {
        Transform3d result;
    
        result.scale = scale * B.scale;
        result.rotation = B.rotation * rotation;
    
        result.position = rotation * (scale * B.position);
        result.position = position + result.position;
    
        (*this) = result;
    }

    Transform3d Transform3d::Inverse(const Transform3d& T)
    {
        Transform3d inverse;
    
        inverse.rotation = Quat::Inverse(T.rotation);
    
        inverse.scale.x = fabs(T.scale.x) < EPSILON ? 0.0f : 1.0f / T.scale.x;
        inverse.scale.y = fabs(T.scale.y) < EPSILON ? 0.0f : 1.0f / T.scale.y;
        inverse.scale.z = fabs(T.scale.z) < EPSILON ? 0.0f : 1.0f / T.scale.z;
    
        Vec3 inverse_translation = T.position * -1.0f;
        inverse.position = inverse.position * (inverse.scale * inverse_translation);
    
        return inverse;
    }

    Transform3d Transform3d::GetInverse()
    {
        Transform3d inverse;
    
        inverse.rotation = Quat::Inverse(rotation);
    
        inverse.scale.x = fabs(scale.x) < EPSILON ? 0.0f : 1.0f / scale.x;
        inverse.scale.y = fabs(scale.y) < EPSILON ? 0.0f : 1.0f / scale.y;
        inverse.scale.z = fabs(scale.z) < EPSILON ? 0.0f : 1.0f / scale.z;
    
        Vec3 inverse_translation = position * -1.0f;
        inverse.position = inverse.position * (inverse.scale * inverse_translation);
        
        return inverse;
    }

    Transform3d Transform3d::Mix(const Transform3d &A, const Transform3d &B, f32 Time)
    {
        Quat bRot = B.rotation;
        if(Quat::Dot(A.rotation, bRot) < 0.0f)
        {
            bRot = -bRot;
        }

        return Transform3d(
            Vec3::Lerp(A.position, B.position, Time),
            Quat::NLerp(A.rotation, bRot, Time),
            Vec3::Lerp(A.scale, B.scale, Time)
        );
    }

    Transform3d Transform3d::Mix(const Transform3d &B, f32 Time)
    {
        Quat bRot = B.rotation;
        if(Quat::Dot(this->rotation, bRot) < 0.0f)
        {
            bRot = -bRot;
        }

        return Transform3d(
            Vec3::Lerp(this->position, B.position, Time),
            Quat::NLerp(this->rotation, bRot, Time),
            Vec3::Lerp(this->scale, B.scale, Time)
        );
    }

    Transform3d Transform3d::FromMat4(const Mat4& Mat)
    {
        Transform3d result;

        result.position = Vec3(Mat[12], Mat[13], Mat[14]);
        result.rotation = Mat4::ToQuat(Mat);

        Mat4 rotScaleMat(
            Mat[0], Mat[1], Mat[2], 0.0f,
            Mat[4], Mat[5], Mat[6], 0.0f,
            Mat[8], Mat[9], Mat[10], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        Mat4 invRotMat = Quat::ToMat4(Quat::Inverse(result.rotation));
        Mat4 scaleSkewmat = rotScaleMat * invRotMat;

        result.scale = Vec3(
            scaleSkewmat[0],
            scaleSkewmat[5],
            scaleSkewmat[10]
        );

        return  result;
    }

    Mat4 Transform3d::ToMat4(const Transform3d &T)
    {
        Vec3 x = T.rotation * Vec3(1.0f, 0.0f, 0.0f);
        Vec3 y = T.rotation * Vec3(0.0f, 1.0f, 0.0f);
        Vec3 z = T.rotation * Vec3(0.0f, 0.0f, 1.0f);

        x = x * T.scale.x;
        y = y * T.scale.y;
        z = z * T.scale.z;

        Vec3 p = T.position;

        return Mat4(
            x.x, x.y, x.z, 0.0f,
            y.x, y.y, y.z, 0.0f,
            z.x, z.y, z.z, 0.0f,
            p.x, p.y, p.z, 1.0f
        );
    }

    Mat4 Transform3d::ToMat4()
    {
        Vec3 x = this->rotation * Vec3(1.0f, 0.0f, 0.0f);
        Vec3 y = this->rotation * Vec3(0.0f, 1.0f, 0.0f);
        Vec3 z = this->rotation * Vec3(0.0f, 0.0f, 1.0f);

        x = x * this->scale.x;
        y = y * this->scale.y;
        z = z * this->scale.z;

        Vec3 p = this->position;

        return Mat4(
            x.x, x.y, x.z, 0.0f,
            y.x, y.y, y.z, 0.0f,
            z.x, z.y, z.z, 0.0f,
            p.x, p.y, p.z, 1.0f
        );
    }

    const Mat4 Transform3d::ToMat4() const
    {
        Vec3 x = this->rotation * Vec3(1.0f, 0.0f, 0.0f);
        Vec3 y = this->rotation * Vec3(0.0f, 1.0f, 0.0f);
        Vec3 z = this->rotation * Vec3(0.0f, 0.0f, 1.0f);

        x = x * this->scale.x;
        y = y * this->scale.y;
        z = z * this->scale.z;

        Vec3 p = this->position;

        return Mat4(
            x.x, x.y, x.z, 0.0f,
            y.x, y.y, y.z, 0.0f,
            z.x, z.y, z.z, 0.0f,
            p.x, p.y, p.z, 1.0f
        );
    }

} // namespace Clunk
