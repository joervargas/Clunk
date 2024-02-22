#include "Quat.h"
#include "Mat4.h"


namespace Clunk
{

    void Quat::SetAngleAxis(f32 Angle, const Vec3& Axis)
    {
        Vec3 norm = Vec3::Normalize(Axis);
        f32 s = sinf(Angle / 2.0f);

        (*this) = Quat(
            norm.X * s,
            norm.Y * s,
            norm.Z * s,
            cosf(Angle / 2.0f)
        );
    }

    Quat Quat::AngleAxis(f32 Angle, const Vec3& Axis)
    {
        Vec3 norm = Vec3::Normalize(Axis);
        f32 s = sinf(Angle / 2.0f);
    
        return Quat(
            norm.X * s,
            norm.Y * s,
            norm.Z * s,
            cosf(Angle / 2.0f)
        );
    }

    Quat Quat::FromTo(const Vec3& From, const Vec3& To)
    {
        Vec3 fn = Vec3::Normalize(From);
        Vec3 tn = Vec3::Normalize(To);
        if(fn == tn)
        {
            return Quat();
        }
        else if (fn == (tn * -1.0f))
        {
            Vec3 ortho = Vec3(1, 0, 0);
            if(fabsf(fn.Y) < fabsf(fn.X))
            {
                ortho = Vec3(0.0f, 1.0f, 0.0f);
            }
            if(fabsf(fn.Z) < fabs(fn.Y) && fabs(fn.Z) < fabsf(fn.X))
            {
                ortho = Vec3(0.0f, 0.0f, 1.0f);
            }
            Vec3 axis = Vec3::Normalize(Vec3::Cross(fn, ortho));
            return Quat(axis, 0.0f);
        }
        Vec3 half = Vec3::Normalize(fn + tn);
        Vec3 axis = Vec3::Cross(fn, half);
        
        return Quat(axis, Vec3::Dot(fn, half));
    }
    
    void Quat::Normalize()
    {
        f32 lensq = this->LengthSquared();
        if(lensq < EPSILON) { return; }
    
        f32 inverse_length = 1.0f / sqrtf(lensq);
        X *= inverse_length;
        Y *= inverse_length;
        Z *= inverse_length;
        W *= inverse_length;
    }

    Quat Quat::GetNormal()
    {
        f32 lensq = this->LengthSquared();
        if(lensq < EPSILON) { return Quat(); }
    
        f32 inverse_length = 1.0f / sqrtf(lensq);
    
        return Quat(
            X * inverse_length,
            Y * inverse_length,
            Z * inverse_length,
            W * inverse_length
        );
    }

    Quat Quat::Inverse(const Quat& Q)
    {
        f32 lensq = Q.LengthSquared();
        if(lensq < EPSILON) { return Quat(); }
    
        f32 inverse_lensq = 1.0f/ lensq;
        return Quat(
            -Q.X * inverse_lensq,
            -Q.Y * inverse_lensq,
            -Q.Z * inverse_lensq,
             Q.W * inverse_lensq
        );
    }

    Quat Quat::GetNormal(const Quat& Q)
    {
        f32 lensq = Q.LengthSquared();
        if(lensq < EPSILON) { return Quat(); }
    
        f32 inverse_length = 1.0f / sqrtf(lensq);
        
        return Quat(
            Q.X * inverse_length,
            Q.Y * inverse_length,
            Q.Z * inverse_length,
            Q.W * inverse_length
        );
    }

    Quat Quat::GetInverse()
    {
        f32 lensq = this->LengthSquared();
        if(lensq < EPSILON) { return Quat(); }

        f32 inverse_lensq = 1.0f/ lensq;
        return Quat(
            -X * inverse_lensq,
            -Y * inverse_lensq,
            -Z * inverse_lensq,
             W * inverse_lensq
        );
    }

    const Quat Quat::GetInverse() const
    {
        f32 lensq = this->LengthSquared();
        if(lensq < EPSILON) { return Quat(); }
    
        f32 inverse_lensq = 1.0f/ lensq;
        return Quat(
            -X * inverse_lensq,
            -Y * inverse_lensq,
            -Z * inverse_lensq,
             W * inverse_lensq
        );
    }

    Quat Quat::GetNeighborhood(const Quat& Other)
    {
        Quat result;
        if(Quat::Dot((*this), Other) < 0.0f)
        {
            result = -Other;
        }
        else
        {
            result = Other;
        }
        return result;
    }

    Quat Quat::LookRotation(const Vec3& Direction, const Vec3& Up)
    {
        Vec3 front = Vec3::Normalize(Direction); // Object Forward
        Vec3 up = Vec3::Normalize(Up); // Desired Up
        Vec3 right = Vec3::Cross(up, front);
        up = Vec3::Cross(front, right);
    
        // TODO: Change world forward to a global variable
        // Vec3 worldForward(0, 0, 1);
        Vec3 worldForward = Vec3::Cross(right, up);
        Quat worldToObject = FromTo(worldForward, front);
    
        // TODO: Change world up to global variable
        // Vec3 worldUp(0, 1, 0);
        Vec3 worldUp = Vec3::Cross(worldForward, right);
        Vec3 objectUp = worldToObject * worldUp;
    
        Quat u2u = FromTo(objectUp, up);
    
        Quat result = worldToObject * u2u;
        return Quat::GetNormal(result);
    }

    Mat4 Quat::ToMat4()
    {
        Vec3 right = (*this) * Vec3(1.f, 0.f, 0.f);
        Vec3 up = (*this) * Vec3(0.f, 1.f, 0.f);
        Vec3 front = (*this) * Vec3(0.f, 0.f, 1.f);

        return Mat4(
            right.X, right.Y, right.Z, 0.0f,
            up.X, up.Y, up.Z, 0.0f,
            front.X, front.Y, front.Z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    Mat4 Quat::ToMat4(const Quat& Q)
    {
        Vec3 right = Q * Vec3(1.f, 0.f, 0.f);
        Vec3 up = Q * Vec3(0.f, 1.f, 0.f);
        Vec3 front = Q * Vec3(0.f, 0.f, 1.f);
    
        return Mat4(
            right.X, right.Y, right.Z, 0.0f,
            up.X, up.Y, up.Z, 0.0f,
            front.X, front.Y, front.Z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }



}

