
#include <ClMath/ClMath.h>

namespace Clunk
{

    enum ClProjectionMode
    {
        Cl_PROJECTION_MODE_Perspective,
        Cl_PROJECTION_MODE_Othographic,
    };

    class ClProjection
    {
    public:

        ClProjection();
        ClProjection(f32 ScreenWidth, f32 ScreenHeight, ClProjectionMode Mode = Cl_PROJECTION_MODE_Perspective, f32 FOV = 45.f, f32 Near = 0.1f, f32 Far = 1000.f) :
            m_ProjectionMode(Mode),
            m_ScreenWidth(ScreenWidth), m_ScreenHeight(ScreenHeight), 
            m_FOV(FOV), 
            m_Near(Near), m_Far(Far)
        {
            if(Mode == Cl_PROJECTION_MODE_Perspective)
            {
                m_Matrix = Mat4::PerspectiveLH(m_FOV, m_ScreenWidth/ m_ScreenHeight, m_Near, m_Far);
            } else {
                m_Matrix = Mat4::OrthographicLH(
                    -(m_ScreenWidth / 2.f), m_ScreenWidth / 2.f,
                    m_ScreenHeight / 2.f, -(m_ScreenHeight / 2.f),
                    m_Near, m_Far
                );
            }
        }

        void SetMode(ClProjectionMode Mode)
        { 
            if(m_ProjectionMode != Mode)
            {
                m_IsDirty = true;
                m_ProjectionMode = Mode;
            }
        }

        void SetScreenWidth(f32 Width)
        {
            if(m_ScreenWidth != Width)
            {
                m_IsDirty = true;
                m_ScreenWidth = Width;
            }
        }

        void SetScreenHeight(f32 Height)
        {
            if(m_ScreenHeight != Height)
            {
                m_IsDirty = true;
                m_ScreenHeight = Height;
            }
        }

        void SetScreenAspect(f32 Width, f32 Height)
        {
            if( m_ScreenWidth != Width || m_ScreenHeight != Height)
            {
                m_IsDirty = true;
                m_ScreenWidth = Width;
                m_ScreenHeight = Height;
            }
        }

        void SetFOV(f32 FOV)
        {
            if(m_FOV != FOV)
            {
                m_IsDirty = true;
                m_FOV = FOV;
            }
        }

        void SetNearPlane(f32 Near)
        {
            if(m_Near != Near)
            {
                m_IsDirty = true;
                m_Near = Near;
            }
        }

        void SetFarPlane(f32 Far)
        {
            if(m_Far != Far)
            {
                m_IsDirty = true;
                m_Far = Far;
            }
        }
        
         void Update()
         {
            if(m_IsDirty)
            {
                m_IsDirty = false;
                if(m_ProjectionMode == Cl_PROJECTION_MODE_Perspective)
                {
                    m_Matrix = Mat4::PerspectiveLH(m_FOV, m_ScreenWidth/ m_ScreenHeight, m_Near, m_Far);
                } else {
                    m_Matrix = Mat4::OrthographicLH(
                        -(m_ScreenWidth / 2.f), m_ScreenWidth / 2.f,
                         m_ScreenHeight / 2.f, -(m_ScreenHeight / 2.f),
                         m_Near, m_Far
                    );
                }
            }
        }

        Mat4 GetProjectionMatrix() const { return m_Matrix; }
        
    private:

        ClProjectionMode m_ProjectionMode = Cl_PROJECTION_MODE_Perspective;

        f32 m_ScreenWidth;
        f32 m_ScreenHeight;
        f32 m_FOV = 45.f;
        f32 m_Near = 0.1f;
        f32 m_Far = 1000.f;
        b8 m_IsDirty = false;
        Mat4 m_Matrix;

    };

}