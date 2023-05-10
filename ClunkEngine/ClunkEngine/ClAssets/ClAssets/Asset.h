#pragma once

#include <PCH/pch.h>

namespace Clunk::Assets
{
    using asset_type_id = u32;
    using asset_id = uuid64;

    struct IAsset
    {
        CLUNK_INLINE virtual ~IAsset() = default;
        asset_id Id = GenerateUUID();
        String Name;
    };
}