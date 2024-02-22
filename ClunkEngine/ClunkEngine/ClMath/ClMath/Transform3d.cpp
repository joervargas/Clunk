#include "Transform3d.h"
#include "Mat4.h"

namespace Clunk
{

    Transform3d Transform3d::Combine(const Transform3d& A, const Transform3d& B)
    {
        Transform3d result;
    
        result.Scale = A.Scale * B.Scale;
        result.Rotation = B.Rotation * A.Rotation;
    
        result.Position = A.Rotation * (A.Scale * B.Position);
        result.Position = A.Position + result.Position;
    
        return result;
    }

    void Transform3d::Combine(const Transform3d& B)
    {
        Transform3d result;
    
        result.Scale = Scale * B.Scale;
        result.Rotation = B.Rotation * Rotation;
    
        result.Position = Rotation * (Scale * B.Position);
        result.Position = Position + result.Position;
    
        (*this) = result;
    }

    Transform3d Transform3d::Inverse(const Transform3d& T)
    {
        Transform3d inverse;
    
        inverse.Rotation = Quat::Inverse(T.Rotation);
    
        inverse.Scale.X = fabs(T.Scale.X) < EPSILON ? 0.0f : 1.0f / T.Scale.X;
        inverse.Scale.Y = fabs(T.Scale.Y) < EPSILON ? 0.0f : 1.0f / T.Scale.Y;
        inverse.Scale.Z = fabs(T.Scale.Z) < EPSILON ? 0.0f : 1.0f / T.Scale.Z;
    
        Vec3 inverse_translation = T.Position * -1.0f;
        inverse.Position = inverse.Position * (inverse.Scale * inverse_translation);
    
        return inverse;
    }

    Transform3d Transform3d::GetInverse()
    {
        Transform3d inverse;
    
        inverse.Rotation = Quat::Inverse(Rotation);
    
        inverse.Scale.X = fabs(Scale.X) < EPSILON ? 0.0f : 1.0f / Scale.X;
        inverse.Scale.Y = fabs(Scale.Y) < EPSILON ? 0.0f : 1.0f / Scale.Y;
        inverse.Scale.Z = fabs(Scale.Z) < EPSILON ? 0.0f : 1.0f / Scale.Z;
    
        Vec3 inverse_translation = Position * -1.0f;
        inverse.Position = inverse.Position * (inverse.Scale * inverse_translation);
        
        return inverse;
    }

    Transform3d Transform3d::Mix(const Transform3d &A, const Transform3d &B, f32 Time)
    {
        Quat bRot = B.Rotation;
        if(Quat::Dot(A.Rotation, bRot) < 0.0f)
        {
            bRot = -bRot;
        }

        return Transform3d(
            Vec3::Lerp(A.Position, B.Position, Time),
            Quat::NLerp(A.Rotation, bRot, Time),
            Vec3::Lerp(A.Scale, B.Scale, Time)
        );
    }

    Transform3d Transform3d::Mix(const Transform3d &B, f32 Time)
    {
        Quat bRot = B.Rotation;
        if(Quat::Dot(this->Rotation, bRot) < 0.0f)
        {
            bRot = -bRot;
        }

        return Transform3d(
            Vec3::Lerp(this->Position, B.Position, Time),
            Quat::NLerp(this->Rotation, bRot, Time),
            Vec3::Lerp(this->Scale, B.Scale, Time)
        );
    }

    Transform3d Transform3d::FromMat4(const Mat4& Mat)
    {
        Transform3d result;

        result.Position = Vec3(Mat[12], Mat[13], Mat[14]);
        result.Rotation = Mat4::ToQuat(Mat);

        Mat4 rotScaleMat(
            Mat[0], Mat[1], Mat[2], 0.0f,
            Mat[4], Mat[5], Mat[6], 0.0f,
            Mat[8], Mat[9], Mat[10], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        Mat4 invRotMat = Quat::ToMat4(Quat::Inverse(result.Rotation));
        Mat4 scale_skewmat = rotScaleMat * invRotMat;

        result.Scale = Vec3(
            scale_skewmat[0],
            scale_skewmat[5],
            scale_skewmat[10]
        );

        return  result;
    }

    Mat4 Transform3d::ToMat4(const Transform3d &T)
    {
        Vec3 x = T.Rotation * Vec3(1.0f, 0.0f, 0.0f);
        Vec3 y = T.Rotation * Vec3(0.0f, 1.0f, 0.0f);
        Vec3 z = T.Rotation * Vec3(0.0f, 0.0f, 1.0f);

        x = x * T.Scale.X;
        y = y * T.Scale.Y;
        z = z * T.Scale.Z;

        Vec3 p = T.Position;

        return Mat4(
            x.X, x.Y, x.Z, 0.0f,
            y.X, y.Y, y.Z, 0.0f,
            z.X, z.Y, z.Z, 0.0f,
            p.X, p.Y, p.Z, 1.0f
        );
    }

    Mat4 Transform3d::ToMat4()
    {
        Vec3 x = this->Rotation * Vec3(1.0f, 0.0f, 0.0f);
        Vec3 y = this->Rotation * Vec3(0.0f, 1.0f, 0.0f);
        Vec3 z = this->Rotation * Vec3(0.0f, 0.0f, 1.0f);

        x = x * this->Scale.X;
        y = y * this->Scale.Y;
        z = z * this->Scale.Z;

        Vec3 p = this->Position;

        return Mat4(
            x.X, x.Y, x.Z, 0.0f,
            y.X, y.Y, y.Z, 0.0f,
            z.X, z.Y, z.Z, 0.0f,
            p.X, p.Y, p.Z, 1.0f
        );
    }

    const Mat4 Transform3d::ToMat4() const
    {
        Vec3 x = this->Rotation * Vec3(1.0f, 0.0f, 0.0f);
        Vec3 y = this->Rotation * Vec3(0.0f, 1.0f, 0.0f);
        Vec3 z = this->Rotation * Vec3(0.0f, 0.0f, 1.0f);

        x = x * this->Scale.X;
        y = y * this->Scale.Y;
        z = z * this->Scale.Z;

        Vec3 p = this->Position;

        return Mat4(
            x.X, x.Y, x.Z, 0.0f,
            y.X, y.Y, y.Z, 0.0f,
            z.X, z.Y, z.Z, 0.0f,
            p.X, p.Y, p.Z, 1.0f
        );
    }

} // namespace Clunk
