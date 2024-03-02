#pragma once

#include "ClFlyCameraMovement.h"
#include "ClProjection.h"

#include <ClPlatforms/ClPlatform.h>

namespace Clunk
{
    // struct ClProjectionView
    // {
    //     Mat4 Projection;
    //     Mat4 View;
    // };

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

        // ClProjectionView GetProjectionViewData() const { return mProjView; }

    private:

        const ICameraMovement* p_CameraMovement;

        // ClProjectionView mProjView;
        
    };

}