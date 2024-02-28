#pragma once

#include "ClFlyCameraMovement.h"
#include "ClProjection.h"

namespace Clunk
{

    class ClCamera
    {
    public:

        ClProjection Projection;

    public:

        ClCamera(f32 ScreenWidth, f32 ScreenHeight, ICameraMovement& CameraMovement) : 
            p_CameraMovement(&CameraMovement)
        {
            Projection = ClProjection(ScreenWidth, ScreenHeight);
        }

        ClCamera(const ClCamera&) = default;
        ClCamera& operator= (const ClCamera&) = default;

        Vec3 GetPosition() const { return p_CameraMovement->GetPosition(); }
        Mat4 GetViewMatrix() const { return p_CameraMovement->GetViewMatrix(); }
        Mat4 GetProjectionMatrix() const { return Projection.GetProjectionMatrix(); }

    private:

        const ICameraMovement* p_CameraMovement;

    };

}