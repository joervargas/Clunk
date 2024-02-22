#include "Mat4.h"
#include "Quat.h"
#include "Transform3d.h"
#include "ClMath.h"
#include <cassert>

namespace Clunk
{
    void Mat4::RotateX(f32 Angle)
    {
        f32 c = std::cos(Angle);
        f32 s = std::sin(Angle);
    
        Mat4 rotMat(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c, s, 0.0f,
            0.0f, -s, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        (*this) = (*this) * rotMat;
    }

    Mat4 Mat4::RotateX(const Mat4& Mat, f32 Angle)
    {
        f32 c = std::cos(Angle);
        f32 s = std::sin(Angle);
    
        Mat4 rotMat(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, c, s, 0.0f,
            0.0f, -s, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        return Mat * rotMat;
    }

    void Mat4::RotateY(f32 Angle)
    {
        f32 c = std::cos(Angle);
        f32 s = std::sin(Angle);
    
        Mat4 rotMat(
            c, 0.0f, -s, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            s, 0.0f, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        (*this) = (*this) * rotMat;
    }

    Mat4 Mat4::RotateY(const Mat4& Mat, f32 Angle)
    {
        f32 c = std::cos(Angle);
        f32 s = std::sin(Angle);
    
        Mat4 rotMat(
            c, 0.0f, -s, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            s, 0.0f, c, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        return Mat * rotMat;
    }

    void Mat4::RotateZ(f32 Angle)
    {
        f32 c = std::cos(Angle);
        f32 s = std::sin(Angle);
    
        Mat4 rotMat(
            c, s, 0.0f, 0.0f,
            -s, c, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        (*this) = (*this) * rotMat;
    }

    Mat4 Mat4::RotateZ(const Mat4& Mat, f32 Angle)
    {
        f32 c = std::cos(Angle);
        f32 s = std::sin(Angle);
    
        Mat4 rotMat(
            c, s, 0.0f, 0.0f,
            -s, c, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        return Mat * rotMat;
    }

    Mat4 Mat4::Scale(const Mat4& Mat, const Vec3& Scale)
    {
        Mat4 s = Mat4::Identity();
        s[0] = Scale.X;
        s[5] = Scale.Y;
        s[10] = Scale.Z;
        return Mat * s;
    }

    void Mat4::Scale(const f32 Scale)
    {
        Mat4 s = Mat4::Identity();
        s[0] = Scale;
        s[5] = Scale;
        s[10] = Scale;
        (*this) = (*this) * s;
    }

    void Mat4::Scale(const Vec3& Scale)
    {
        Mat4 s = Mat4::Identity();
        s[0] = Scale.X;
        s[5] = Scale.Y;
        s[10] = Scale.Z;
        (*this) = (*this) * s;
    }

    Mat4 Mat4::Scale(const Mat4& Mat, const f32 Scale)
    {
        Mat4 s = Mat4::Identity();
        s[0] = Scale;
        s[5] = Scale;
        s[10] = Scale;
        return Mat * s;
    }

    Mat4 Mat4::Translate(const Mat4& Mat, const Vec3& Translation)
    {
        Mat4 result = Mat;
        result[12] = Translation.X;
        result[13] = Translation.Y;
        result[14] = Translation.Z;
        return result;
    }

    void Mat4::Translate(const Vec3& Translation)
    {
        Data[12] = Translation.X;
        Data[13] = Translation.Y;
        Data[14] = Translation.Z;
    }

    Mat4 Mat4::ReflectionMatrix(const Vec3& Axis)
    {
        f32 x = Axis.X * -2.0f;
        f32 y = Axis.Y * -2.0f;
        f32 z = Axis.Z * -2.0f;
        f32 axay = x * Axis.Y;
        f32 axaz = x * Axis.Z;
        f32 ayaz = y * Axis.Z;
    
        return Mat4(
            x * Axis.X + 1.0f, axay, axaz, 0.0f,
            axay, y * Axis.Y + 1.0f, ayaz, 0.0f,
            axaz, ayaz, z* Axis.Z + 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    Mat4 Mat4::LookAtRH(const Vec3& Position, const Vec3& Target, const Vec3& Up)
    {
        Vec3 const front(Vec3::Normalize(Target - Position));
		Vec3 const right(Vec3::Normalize(Vec3::Cross(front, Up))); // right handed
		Vec3 const up(Vec3::Normalize(Vec3::Cross(right, front))); // right handed

		Vec3 target(
			Vec3::Dot(right, Position),
			Vec3::Dot(up, Position),
			Vec3::Dot(front, Position)
		);

		// Transpose upper 3x3 matrix to invert it
		return Mat4(
			right.X, up.X, -front.X, 0.0f,
			right.Y, up.Y, -front.Y, 0.0f,
			right.Z, up.Z, -front.Z, 0.0f,
			-target.X, -target.Y, target.Z, 1.0f
		);
    }

    Mat4 Mat4::LookAtLH(const Vec3& Position, const Vec3& Target, const Vec3& Up)
    {
        Vec3 const front(Vec3::Normalize(Target - Position));
		Vec3 const right(Vec3::Normalize(Vec3::Cross(Up, front))); // left
		Vec3 const up(Vec3::Normalize(Vec3::Cross(front, right))); // left

		Vec3 target(
			Vec3::Dot(right, Position),
			Vec3::Dot(up, Position),
			Vec3::Dot(front, Position)
		);

		// Transpose upper 3x3 matrix to invert it
		return Mat4(
			right.X, up.X, front.X, 0.0f,
			right.Y, up.Y, front.Y, 0.0f,
			right.Z, up.Z, front.Z, 0.0f,
			-target.X, -target.Y, -target.Z, 1.0f
		);
    }

    Mat4 Mat4::PerspectiveRH(f32 FOV, f32 AspectRatio, f32 Near, f32 Far)
    {
		f32 half_tan_fov = tanf(Math::DegToRad(FOV) / 2.0f);

		Mat4 result;
		result.Data[0] = 1.0f / (AspectRatio * half_tan_fov);
		result.Data[5] = 1.0f / half_tan_fov;
		result.Data[10] = - (Far + Near) / (Far - Near);
		result.Data[11] = - 1.0f;
		result.Data[14] = -(2.0f * Far * Near) / (Far - Near);
		return result;
    }


    Mat4 Mat4::PerspectiveLH(f32 FOV, f32 AspectRatio, f32 Near, f32 Far)
    {
		f32 focal_length = 1.0f / tanf( Math::DegToRad(FOV) * 0.5f);

		Mat4 result;
		result.Data[0] = focal_length / AspectRatio;
		result.Data[5] = focal_length;
		result.Data[10] = (Far + Near) / (Far - Near);
		result.Data[11] = 1.0f;
		result.Data[14] = (2.0f * Far * Near) / (Near - Far);
		return result;
    }


    Mat4 Mat4::PerspectiveVK(f32 FOV, f32 AspectRatio, f32 Near, f32 Far)
    {
		f32 focal_length = 1.0f / tanf( Math::DegToRad(FOV) * 0.5f);

		Mat4 result;
		result.Data[0] = focal_length / AspectRatio;
		result.Data[5] = -focal_length;
		result.Data[10] = Far / (Near - Far);
		result.Data[11] = - 1.0f;
		result.Data[14] = (Near * Far) / (Near - Far);
		return result;
    }

    Mat4 Mat4::OrthographicRH(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far)
    {
		if(Left == Right || Top == Bottom || Near == Far)
		{
			throw std::runtime_error("Invalid frustum!\n");
			return Mat4();
		}

		return Mat4(
			2.0f / (Right - Left), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / (Top - Bottom), 0.0f, 0.0f,
			0.0f, 0.0f, -2.0f / (Far - Near), 0.0f,
			-((Right+Left)/(Right-Left)), -((Top+Bottom)/(Top-Bottom)), -((Far+Near)/(Far-Near)), 1.0f
		);
    }

    Mat4 Mat4::OrthographicLH(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far)
    {
        if(Left == Right || Top == Bottom || Near == Far)
		{
			throw std::runtime_error("Invalid frustum!\n");
			return Mat4();
		}

		return Mat4(
			2.0f / (Right - Left), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / (Top - Bottom), 0.0f, 0.0f,
			0.0f, 0.0f, 2.0f / (Far - Near), 0.0f,
			-((Right+Left)/(Right-Left)), -((Top+Bottom)/(Top-Bottom)), -((Far+Near)/(Far-Near)), 1.0f
		);
    }

    Mat4 Mat4::OrthographicVK(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far)
    {
        if(Left == Right || Top == Bottom || Near == Far)
		{
			throw std::runtime_error("Invalid frustum!\n");
			return Mat4();
		}

		return Mat4(
			2.0f / (Right - Left), 0.0f, 0.0f, 0.0f,
			0.0f, 2.0f / (Top - Bottom), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (Far - Near), 0.0f,
			-((Right+Left)/(Right-Left)), -((Bottom + Top)/(Bottom- Top)), (Near)/(Near-Far), 1.0f
		);
    }

    Mat4 Mat4::Inverse(const Mat4& Mat)
    {
        f32 t0 = Mat[10] * Mat[15];
        f32 t1 = Mat[14] * Mat[11];
        f32 t2 = Mat[6] * Mat[15];
        f32 t3 = Mat[14] * Mat[7];
        f32 t4 = Mat[6] * Mat[11];
        f32 t5 = Mat[10] * Mat[7];
        f32 t6 = Mat[2] * Mat[15];
        f32 t7 = Mat[14] * Mat[3];
        f32 t8 = Mat[2] * Mat[11];
        f32 t9 = Mat[10] * Mat[3];
        f32 t10 = Mat[2] * Mat[7];
        f32 t11 = Mat[6] * Mat[3];
        f32 t12 = Mat[8] * Mat[13];
        f32 t13 = Mat[12] * Mat[9];
        f32 t14 = Mat[4] * Mat[13];
        f32 t15 = Mat[12] * Mat[5];
        f32 t16 = Mat[4] * Mat[9];
        f32 t17 = Mat[8] * Mat[5];
        f32 t18 = Mat[0] * Mat[13];
        f32 t19 = Mat[12] * Mat[1];
        f32 t20 = Mat[0] * Mat[9];
        f32 t21 = Mat[8] * Mat[1];
        f32 t22 = Mat[0] * Mat[5];
        f32 t23 = Mat[4] * Mat[1];
    
        Mat4 result;
        result[0] = (t0 * Mat[5] + t3 * Mat[9] + t4 * Mat[13]) - (t1 * Mat[5] + t2 * Mat[9] + t5 * Mat[13]);
        result[1] = (t1 * Mat[1] + t6 * Mat[9] + t9 * Mat[13]) - (t0 * Mat[1] + t7 * Mat[9] + t8 * Mat[13]);
        result[2] = (t2 * Mat[1] + t7 * Mat[5] + t10 * Mat[13]) - (t3 * Mat[1] + t6 * Mat[5] + t11 * Mat[13]);
        result[3] = (t5 * Mat[1] + t8 * Mat[5] + t11 * Mat[9]) - (t4 * Mat[1] + t9 * Mat[5] + t10 * Mat[9]);
    
        f32 determinant = 1.0f / (Mat[0] * result[0] + Mat[4] * result[1] + Mat[8] * result[2] + Mat[12] * result[3]);
    
        result[0] = determinant * result[0];
        result[1] = determinant * result[1];
        result[2] = determinant * result[2];
        result[3] = determinant * result[3];
        result[4] = determinant * ((t1 * Mat[4] + t2 * Mat[8] + t5 * Mat[12]) - (t0 * Mat[4] + t3 * Mat[8] + t4 * Mat[12]));
        result[5] = determinant * ((t0 * Mat[0] + t7 * Mat[8] + t8 * Mat[12]) - (t1 * Mat[0] + t6 * Mat[8] + t9 * Mat[12]));
        result[6] = determinant * ((t3 * Mat[0] + t6 * Mat[4] + t11 * Mat[12]) - (t2 * Mat[0] + t7 * Mat[4] + t10 * Mat[12]));
        result[7] = determinant * ((t4 * Mat[0] + t9 * Mat[4] + t10 * Mat[8]) - (t5 * Mat[0] + t8 * Mat[4] + t11 * Mat[8]));
        result[8] = determinant * ((t12 * Mat[7] + t15 * Mat[11] + t16 * Mat[15]) - (t13 * Mat[7] + t14 * Mat[11] + t17 * Mat[15]));
        result[9] = determinant * ((t13 * Mat[3] + t18 * Mat[11] + t21 * Mat[15]) - (t12 * Mat[3] + t19 * Mat[11] + t20 * Mat[15]));
        result[10] = determinant * ((t14 * Mat[3] + t19 * Mat[7] + t22 * Mat[15]) - (t15 * Mat[3] + t18 * Mat[7] + t23 * Mat[15]));
        result[11] = determinant * ((t17 * Mat[3] + t20 * Mat[7] + t23 * Mat[11]) - (t16 * Mat[3] + t21 * Mat[7] + t22 * Mat[11]));
        result[12] = determinant * ((t14 * Mat[10] + t17 * Mat[14] + t13 * Mat[6]) - (t16 * Mat[14] + t12 * Mat[6] + t15 * Mat[10]));
        result[13] = determinant * ((t20 * Mat[14] + t12 * Mat[2] + t19 * Mat[10]) - (t18 * Mat[10] + t21 * Mat[14] + t13 * Mat[2]));
        result[14] = determinant * ((t18 * Mat[6] + t23 * Mat[14] + t15 * Mat[2]) - (t22 * Mat[14] + t14 * Mat[2] + t19 * Mat[6]));
        result[15] = determinant * ((t22 * Mat[10] + t16 * Mat[2] + t21 * Mat[6]) - (t20 * Mat[6] + t23 * Mat[10] + t17 * Mat[2]));
    
        return result;
    }

    void Mat4::Invert()
    {
        f32 t0 = Data[10] * Data[15];
        f32 t1 = Data[14] * Data[11];
        f32 t2 = Data[6] * Data[15];
        f32 t3 = Data[14] * Data[7];
        f32 t4 = Data[6] * Data[11];
        f32 t5 = Data[10] * Data[7];
        f32 t6 = Data[2] * Data[15];
        f32 t7 = Data[14] * Data[3];
        f32 t8 = Data[2] * Data[11];
        f32 t9 = Data[10] * Data[3];
        f32 t10 = Data[2] * Data[7];
        f32 t11 = Data[6] * Data[3];
        f32 t12 = Data[8] * Data[13];
        f32 t13 = Data[12] * Data[9];
        f32 t14 = Data[4] * Data[13];
        f32 t15 = Data[12] * Data[5];
        f32 t16 = Data[4] * Data[9];
        f32 t17 = Data[8] * Data[5];
        f32 t18 = Data[0] * Data[13];
        f32 t19 = Data[12] * Data[1];
        f32 t20 = Data[0] * Data[9];
        f32 t21 = Data[8] * Data[1];
        f32 t22 = Data[0] * Data[5];
        f32 t23 = Data[4] * Data[1];
    
        Mat4 result;
        result[0] = (t0 * Data[5] + t3 * Data[9] + t4 * Data[13]) - (t1 * Data[5] + t2 * Data[9] + t5 * Data[13]);
        result[1] = (t1 * Data[1] + t6 * Data[9] + t9 * Data[13]) - (t0 * Data[1] + t7 * Data[9] + t8 * Data[13]);
        result[2] = (t2 * Data[1] + t7 * Data[5] + t10 * Data[13]) - (t3 * Data[1] + t6 * Data[5] + t11 * Data[13]);
        result[3] = (t5 * Data[1] + t8 * Data[5] + t11 * Data[9]) - (t4 * Data[1] + t9 * Data[5] + t10 * Data[9]);
    
        f32 determinant = 1.0f / (Data[0] * result[0] + Data[4] * result[1] + Data[8] * result[2] + Data[12] * result[3]);
    
        result[0] = determinant * result[0];
        result[1] = determinant * result[1];
        result[2] = determinant * result[2];
        result[3] = determinant * result[3];
        result[4] = determinant * ((t1 * Data[4] + t2 * Data[8] + t5 * Data[12]) - (t0 * Data[4] + t3 * Data[8] + t4 * Data[12]));
        result[5] = determinant * ((t0 * Data[0] + t7 * Data[8] + t8 * Data[12]) - (t1 * Data[0] + t6 * Data[8] + t9 * Data[12]));
        result[6] = determinant * ((t3 * Data[0] + t6 * Data[4] + t11 * Data[12]) - (t2 * Data[0] + t7 * Data[4] + t10 * Data[12]));
        result[7] = determinant * ((t4 * Data[0] + t9 * Data[4] + t10 * Data[8]) - (t5 * Data[0] + t8 * Data[4] + t11 * Data[8]));
        result[8] = determinant * ((t12 * Data[7] + t15 * Data[11] + t16 * Data[15]) - (t13 * Data[7] + t14 * Data[11] + t17 * Data[15]));
        result[9] = determinant * ((t13 * Data[3] + t18 * Data[11] + t21 * Data[15]) - (t12 * Data[3] + t19 * Data[11] + t20 * Data[15]));
        result[10] = determinant * ((t14 * Data[3] + t19 * Data[7] + t22 * Data[15]) - (t15 * Data[3] + t18 * Data[7] + t23 * Data[15]));
        result[11] = determinant * ((t17 * Data[3] + t20 * Data[7] + t23 * Data[11]) - (t16 * Data[3] + t21 * Data[7] + t22 * Data[11]));
        result[12] = determinant * ((t14 * Data[10] + t17 * Data[14] + t13 * Data[6]) - (t16 * Data[14] + t12 * Data[6] + t15 * Data[10]));
        result[13] = determinant * ((t20 * Data[14] + t12 * Data[2] + t19 * Data[10]) - (t18 * Data[10] + t21 * Data[14] + t13 * Data[2]));
        result[14] = determinant * ((t18 * Data[6] + t23 * Data[14] + t15 * Data[2]) - (t22 * Data[14] + t14 * Data[2] + t19 * Data[6]));
        result[15] = determinant * ((t22 * Data[10] + t16 * Data[2] + t21 * Data[6]) - (t20 * Data[6] + t23 * Data[10] + t17 * Data[2]));
    
        (*this) = result;
    }

    Quat Mat4::ToQuat(const Mat4& Mat)
    {
        Vec3 upv = Vec3::Normalize(Vec3(Mat.Up.X, Mat.Up.Y, Mat.Up.Z));
        Vec3 forwardv = Vec3::Normalize(Vec3(Mat.Forward.X, Mat.Forward.Y, Mat.Forward.Z));
        Vec3 rightv = Vec3::Cross(upv, forwardv);
        upv = Vec3::Cross(forwardv, rightv);
    
        return Quat::LookRotation(forwardv, upv);
    }

    Quat Mat4::ToQuat()
    {
		Vec3 upv = Vec3::Normalize(Vec3(this->Up.X, this->Up.Y, this->Up.Z));
        Vec3 forwardv = Vec3::Normalize(Vec3(this->Forward.X, this->Forward.Y, this->Forward.Z));
        Vec3 rightv = Vec3::Cross(upv, forwardv);
        upv = Vec3::Cross(forwardv, rightv);
    
        return Quat::LookRotation(forwardv, upv);
    }

    const Quat Mat4::ToQuat() const
    {
        Vec3 upv = Vec3::Normalize(Vec3(this->Up.X, this->Up.Y, this->Up.Z));
        Vec3 forwardv = Vec3::Normalize(Vec3(this->Forward.X, this->Forward.Y, this->Forward.Z));
        Vec3 rightv = Vec3::Cross(upv, forwardv);
        upv = Vec3::Cross(forwardv, rightv);
    
        return Quat::LookRotation(forwardv, upv);
    }

    Transform3d Mat4::ToTransform3d(const Mat4& Mat)
    {
        Transform3d result;

        result.Position = Vec3(Mat[12], Mat[13], Mat[14]);
        result.Rotation = Mat.ToQuat();

        Mat4 rotScaleMat(
            Mat[0], Mat[1], Mat[2], 0.0f,
            Mat[4], Mat[5], Mat[6], 0.0f,
            Mat[8], Mat[9], Mat[10], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        Mat4 invRotMat = Quat::ToMat4(Quat::Inverse(result.Rotation));
        Mat4 scaleSkewmat = rotScaleMat * invRotMat;

        result.Scale = Vec3(
            scaleSkewmat[0],
            scaleSkewmat[5],
            scaleSkewmat[10]
        );

        return  result;
    }

    Transform3d Mat4::ToTransform3d()
    {
        Transform3d result;

        result.Position = Vec3(Data[12], Data[13], Data[14]);
        result.Rotation = this->ToQuat();

        Mat4 rotScaleMat(
            Data[0], Data[1], Data[2], 0.0f,
            Data[4], Data[5], Data[6], 0.0f,
            Data[8], Data[9], Data[10], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        Mat4 invRotMat = Quat::ToMat4(Quat::Inverse(result.Rotation));
        Mat4 scaleSkewmat = rotScaleMat * invRotMat;

        result.Scale = Vec3(
            scaleSkewmat[0],
            scaleSkewmat[5],
            scaleSkewmat[10]
        );

        return  result;
    }

    const Transform3d Mat4::ToTransform3d() const
    {
        Transform3d result;

        result.Position = Vec3(Data[12], Data[13], Data[14]);
        result.Rotation = this->ToQuat();

        Mat4 rotScaleMat(
            Data[0], Data[1], Data[2], 0.0f,
            Data[4], Data[5], Data[6], 0.0f,
            Data[8], Data[9], Data[10], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        Mat4 invRotMat = Quat::ToMat4(Quat::Inverse(result.Rotation));
        Mat4 scaleSkewmat = rotScaleMat * invRotMat;

        result.Scale = Vec3(
            scaleSkewmat[0],
            scaleSkewmat[5],
            scaleSkewmat[10]
        );

        return  result;
    }

    Mat4 Mat4::FromTransform3d(const Transform3d &T)
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
}
