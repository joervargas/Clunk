

#include "ICameraMovement.h"
#include <Core/Logger.h>
#include <ClPlatforms/ClPlatform.h>

namespace Clunk
{
    class ClFlyCameraMovement : public ICameraMovement
    {
    public:

        struct Speed
        {
            Vec2 Mouse = Vec2(4.0f);
            f32 Movement = 1.0f;
            f32 Acceleration = 20.f;
            f32 Damping = 0.2f;
            f32 MaxSpeed = 10.f;
            f32 FastCoef = 10.f;
        } Speed;

        struct Movement
        {
            b8 Forward = false;
            b8 Backward = false;
            b8 Left = false;
            b8 Right = false;
            b8 Up = false;
            b8 Down = false;
            b8 Fast = false;
        } Movement;

    public:

        ClFlyCameraMovement() = default;
        ClFlyCameraMovement(const Vec3& Pos, const Vec3& Target = Vec3(0.f, 1.f, 0.f), const Vec3& Up = Vec3(0.0f, 0.0f, 1.0f)) :
            m_CameraPos(Pos), m_Forward(Target), m_Up(Up), m_WorldUp(Up),
            m_CameraOrientaion(Quat::LookRotation(Target, m_Up))
        {
            UpdateVectors();
            m_CameraOrientaion.LookRotaion(Target, m_Up);
        }

        virtual void Update(const Vec2& MousePos, f32 DeltaTime)
        {
            // mouse movement
            // const Vec2 mouse_delta = MousePos - m_MousePos;
            // const Quat quat_delta = Quat(Vec3(-MouseSpeed * mouse_delta.Y, MouseSpeed * mouse_delta.X, 0.0f));
            // m_CameraOrientaion = quat_delta * m_CameraOrientaion;
            f32 yaw_offset = MousePos.X * Speed.Mouse.X;
            f32 pitch_offset = MousePos.Y * Speed.Mouse.Y;

            m_Yaw -= yaw_offset;
            m_Pitch += pitch_offset;

            Math::Clamp(m_Pitch, -89.0f, 89.0f);

            // f32 velocity = Speed.Movement * DeltaTime;
            Vec3 pos(0.f);
            if(Movement.Forward) { pos += m_Forward; }
            if(Movement.Backward) { pos -= m_Forward; }
            
            if(Movement.Left) { pos -= m_Right; }
            if(Movement.Right) { pos += m_Right; }
            
            if(Movement.Up) { pos += m_Up; }
            if(Movement.Down) { pos -= m_Up; }

            if(pos == Vec3(0.f)) 
            {
                // decelerate
                // pos -= pos * std::min((1.0f / Speed.Damping) * DeltaTime, 1.0f);
                m_CurrentSpeed -= m_CurrentSpeed * std::min((1.0f / Speed.Damping) * DeltaTime, 1.0f);
            } else {
                // accelerate
                m_CurrentSpeed += pos * Speed.Acceleration * DeltaTime;
                const float max_speed =  Movement.Fast ? Speed.MaxSpeed * Speed.FastCoef : Speed.MaxSpeed;
                if(m_CurrentSpeed.Length() > max_speed) m_CurrentSpeed = m_CurrentSpeed.GetNormal() * Speed.MaxSpeed;
            }
            m_CameraPos += m_CurrentSpeed * DeltaTime;

            UpdateVectors();

            m_CameraOrientaion.LookRotation(m_Forward, m_Up);
        }

        virtual Mat4 GetViewMatrix() const { return m_CameraOrientaion.ToMat4(); }

        virtual Vec3 GetPosition() const { return m_CameraPos; };

        virtual void MoveForward() { Movement.Forward = true; }
        virtual void MoveBackward() { Movement.Backward = true; }
        virtual void MoveRight() { Movement.Right = true; }
        virtual void MoveLeft() { Movement.Left = true; }
        virtual void MoveUp() { Movement.Up = true; }
        virtual void MoveDown() { Movement.Down = true; }
        virtual void MoveFast() { Movement.Fast = true; }

    protected:

        f32 m_Yaw = -90.0f; // Left/Right
        f32 m_Pitch = 0.f; // Up/Down

        Vec3 m_CurrentSpeed = Vec3(0.f);

        Vec3 m_CameraPos = Vec3(0.0f, 10.0f, 10.0f);
        Vec3 m_Forward = Vec3(0.f, 1.f, 0.f);
        Vec3 m_Right = Vec3(-1.f, 0.f, 0.f);
        Vec3 m_Up = Vec3(0.f, 0.f, 1.0f);
        Vec3 m_WorldUp = Vec3(0.f, 0.f, 1.0f);
        Quat m_CameraOrientaion = Quat(Vec3(), 0);

    protected:

        void UpdateVectors()
        {
            // const Mat4 view = GetViewMatrix();
            // const Vec3 dir = -Vec3(view.Row[0][2], view.Row[1][2], view.Row[2][2]);

            // m_CameraOrientaion = Quat::LookRotation( m_CameraPos + dir, Up);

            m_Forward = Vec3(
                cosf(Math::DegToRad(m_Yaw)) * cosf(Math::DegToRad(m_Pitch)),
                sinf(Math::DegToRad(m_Yaw)) * cosf(Math::DegToRad(m_Pitch)),
                sinf(Math::DegToRad(m_Pitch))
            );
            m_Forward.Normalize();
            m_Right = Vec3::Cross(m_Forward, m_WorldUp);
            m_Up = Vec3::Cross(m_Right, m_Forward);
        }



    };

} 