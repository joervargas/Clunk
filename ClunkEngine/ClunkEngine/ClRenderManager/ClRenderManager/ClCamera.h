#pragma once

#include <Core/Logger.h>
#include <ClMath/ClMath.h>


namespace Clunk
{
    class ICameraMovement
    {
    public:

        virtual ~ICameraMovement() = default;

        virtual Mat4 GetViewMatrix() const = 0;
        virtual Vec3 GetPosition() const = 0;

    };


    class CameraMovementFirstPerson : public ICameraMovement
    {
    public:

        struct Movement
        {
            b8 Forward = false;
            b8 Backward = false;
            b8 Left = false;
            b8 Right = false;
            b8 Up = false;
            b8 Down = false;
        } Movement;

        f32 MouseSpeed = 4.0f;
        f32 Acceleration = 150.f;
        f32 Damping = 0.2f;
        f32 MaxSpeed = 10.f;
        f32 FastCoef = 10.f;

        Vec3 MoveSpeed = Vec3(1.0f);

    public:

        CameraMovementFirstPerson() = default;
        CameraMovementFirstPerson(const Vec3& Pos, const Vec3& Target, const Vec3& Up) :
            m_CameraPos(Pos), m_Up(Up),
            m_CameraOrientaion(Quat::LookRotation(Target, Up))
        {}

        virtual void Update(const Vec2& MousePos, f32 DeltaTime)
        {
            // mouse movement
            const Vec2 mouse_delta = MousePos - m_MousePos;
            const Quat quat_delta = Quat(Vec3(-MouseSpeed * mouse_delta.Y, MouseSpeed * mouse_delta.X, 0.0f));
            m_CameraOrientaion = quat_delta * m_CameraOrientaion;
        }

        virtual Mat4 GetViewMatrix() const;

        virtual Vec3 GetPosition() const;

    protected:

        void SetUpVector(const Vec3& Up)
        {
            const Mat4 view = GetViewMatrix();
            const Vec3 dir = -Vec3(view.Row[0][2], view.Row[1][2], view.Row[2][2]);

            m_CameraOrientaion = Quat::LookRotation( m_CameraPos + dir, Up);
        }

    private:

        Vec2 m_MousePos = Vec2();

        Vec3 m_CameraPos = Vec3(0.0f, 10.0f, 10.0f);
        Vec3 m_Up = Vec3(0.f, 0.f, 1.0f);
        Quat m_CameraOrientaion = Quat(Vec3(), 0);


    };

}