#pragma once

#include <ClMath/ClMath.h>
#include "ClBitmap.h"

namespace Clunk
{
    ClBitmap convertEquirectangularMapToVerticalCross(const ClBitmap& b);
    ClBitmap convertVerticalCrossToCubeMapFaces(const ClBitmap& b);

    inline ClBitmap convertEquirectangularMapToCubeMapFaces(const ClBitmap& b)
    {
        return convertVerticalCrossToCubeMapFaces(convertEquirectangularMapToVerticalCross(b));
    }
}