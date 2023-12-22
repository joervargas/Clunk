#pragma once

#include <PCH/pch.h>
#include <ClMath/ClMath.h>

namespace Clunk
{
    enum EBitmapType
    {
        EBitmapType_2D,
        EBitmapType_Cube
    };

    enum EBitmapFormat
    {
        EBitmapFormat_UnsignedByte,
        EBitmapFormat_Float
    };

    class Bitmap
    {
    public:

        Bitmap() = default;
        Bitmap(i32 Width, i32 Height, i32 Components, EBitmapFormat Format) :
            mWidth(Width), mHeight(Height),
            mComponents(Components), mFormat(Format),
            mData(Width * Height * Components * GetBytesPerComponent(Format))
        {
            // initGetSetFuncs()
        }

        Bitmap(i32 Width, i32 Height, i32 Depth, i32 Components, EBitmapFormat Format) :
            mWidth(Width), mHeight(Height), mDepth(Depth),
            mComponents(Components), mFormat(Format),
            mData(Width * Height * Components * GetBytesPerComponent(Format))
        {
            // initGetSetFuncs()
        }

        Bitmap(i32 Width, i32 Height, i32 Depth, i32 Components, EBitmapFormat Format, const void* pData) :
            mWidth(Width), mHeight(Height), mDepth(Depth),
            mComponents(Components), mFormat(Format),
            mData(Width * Height * Components * GetBytesPerComponent(Format))
        {
            // initGetSetFuncs()
            memcpy(mData.data(), pData, mData.size());
        }

        static int GetBytesPerComponent(EBitmapFormat Format)
        {
            if(Format == EBitmapFormat_UnsignedByte) return 1;
            if(Format == EBitmapFormat_Float) return 4;
            return 0;
        }

    private: // variables

        i32 mWidth = 0;
        i32 mHeight = 0;
        i32 mDepth = 0;
        i32 mComponents = 0; // count of R,G,B,A values present in a pixel
        EBitmapFormat mFormat = EBitmapFormat_UnsignedByte;
        EBitmapType mType = EBitmapType_2D;

        std::vector<u8> mData;

    private: // functions

        void setPixelFloat(i32 X, i32 Y, const Vec4& Value)
        {
            const int offset = mComponents * (Y * mWidth + X);
            float* data = reinterpret_cast<float*>(mData.data());
            if(mComponents > 0) data[offset + 0] = Value.x;
            if(mComponents > 1) data[offset + 1] = Value.y;
            if(mComponents > 2) data[offset + 2] = Value.z;
            if(mComponents > 3) data[offset + 3] = Value.w;
        }

        Vec4 getPixelFloat(i32 X, i32 Y) const
        {
            const int offset = mComponents * ( Y * mWidth + X);
            const float* data = reinterpret_cast<const float*>(mData.data());
            return Vec4(
                mComponents > 0 ? data[offset + 0] : 0.0f,
                mComponents > 1 ? data[offset + 1] : 0.0f,
                mComponents > 2 ? data[offset + 2] : 0.0f,
                mComponents > 3 ? data[offset + 3] : 0.0f
            );
        }

        void setPixelUnsignedByte(i32 X, i32 Y, const Vec4& Value)
        {}

        Vec4 getPixelUnsignedByte(i32 X, i32 Y) const
        {}

    };

}
