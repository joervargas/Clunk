
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
}
