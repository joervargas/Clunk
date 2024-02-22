#pragma once

#include <PCH/pch.h>
#include <ClMath/ClMath.h>
// #include <glm/glm.hpp>


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

    class ClBitmap
    {
    public:

        std::vector<u8> Data;

        ClBitmap() = default;
        ClBitmap(i32 Width, i32 Height, i32 Channels, EBitmapFormat Format) :
            Data(Width * Height * Channels * GetBytesPerComponent(Format)),
            mWidth(Width), mHeight(Height),
            mChannels(Channels), mFormat(Format)
        {
            initGetSetFuncs();
        }

        ClBitmap(i32 Width, i32 Height, i32 Channels, i32 Layers, EBitmapFormat Format) :
            Data(Width * Height * Layers * Channels * GetBytesPerComponent(Format)),
            mWidth(Width), mHeight(Height), mLayers(Layers),
            mChannels(Channels), mFormat(Format)
        {
            initGetSetFuncs();
        }

        ClBitmap(i32 Width, i32 Height, i32 Channels, EBitmapFormat Format, const void* pData) :
            Data(Width * Height * Channels * GetBytesPerComponent(Format)),
            mWidth(Width), mHeight(Height),
            mChannels(Channels), mFormat(Format)
        {
            initGetSetFuncs();
            memcpy(Data.data(), pData, Data.size());
        }

        static int GetBytesPerComponent(EBitmapFormat Format)
        {
            if(Format == EBitmapFormat_UnsignedByte) return 1;
            if(Format == EBitmapFormat_Float) return 4;
            return 0;
        }

        void SetPixel(int X, int Y, const Vec4& Value)
        {
            (*this.*setPixelFunc)(X, Y, Value);
        }

        Vec4 GetPixel(int X, int Y) const
        {
            return (*this.*getPixelFunc)(X, Y);
        }

        // void SetPixel(int X, int Y, const glm::vec4& Value)
        // {
        //     (*this.*setPixelFunc)(X, Y, Value);
        // }
        
        // glm::vec4 GetPixel(int X, int Y) const
        // {
        //     return (*this.*getPixelFunc)(X, Y);
        // }

        inline EBitmapType GetType() const { return mType; }
        inline void SetType(EBitmapType T) { mType = T; }
        inline EBitmapFormat GetFormat() const { return mFormat; }
        inline i32 GetWidth() const { return mWidth; }
        inline i32 GetHeight() const { return mHeight; }
        inline i32 GetLayers() const { return mLayers; }
        inline i32 GetChannels() const { return mChannels; }

    private: // variables

        i32 mWidth = 0;
        i32 mHeight = 0;
        i32 mLayers = 1;
        i32 mChannels = 3; // count of R,G,B,A values present in a pixel
        EBitmapFormat mFormat = EBitmapFormat_UnsignedByte;
        EBitmapType mType = EBitmapType_2D;

        // Function pointers depending on mData's type
        using setPixel_t = void(ClBitmap::*)(int, int, const Vec4&);
        using getPixel_t = Vec4(ClBitmap::*)(int, int) const;
        // using setPixel_t = void(ClBitmap::*)(int, int, const glm::vec4&);
        // using getPixel_t = glm::vec4(ClBitmap::*)(int, int) const;
        setPixel_t setPixelFunc = &ClBitmap::setPixelUnsignedByte;
        getPixel_t getPixelFunc = &ClBitmap::getPixelUnsignedByte;

    private: // functions

        void initGetSetFuncs()
        {
            switch (mFormat)
            {
            case EBitmapFormat_UnsignedByte:
                setPixelFunc = &ClBitmap::setPixelUnsignedByte;
                getPixelFunc = &ClBitmap::getPixelUnsignedByte;
                break;
            case EBitmapFormat_Float:
                setPixelFunc = &ClBitmap::setPixelFloat;
                getPixelFunc = &ClBitmap::getPixelFloat;
                break;
            }
        }

        void setPixelFloat(i32 X, i32 Y, const Vec4& Value)
        {
            const int offset = mChannels * (Y * mWidth + X);
            float* data = reinterpret_cast<float*>(Data.data());
            if(mChannels > 0) data[offset + 0] = Value.X;
            if(mChannels > 1) data[offset + 1] = Value.Y;
            if(mChannels > 2) data[offset + 2] = Value.Z;
            if(mChannels > 3) data[offset + 3] = Value.W;
        }

        // void setPixelFloat(i32 X, i32 Y, const glm::vec4& Value)
        // {
        //     const int offset = mChannels * (Y * mWidth + X);
        //     float* data = reinterpret_cast<float*>(Data.data());
        //     if(mChannels > 0) data[offset + 0] = Value.x;
        //     if(mChannels > 1) data[offset + 1] = Value.y;
        //     if(mChannels > 2) data[offset + 2] = Value.z;
        //     if(mChannels > 3) data[offset + 3] = Value.w;
        // }

        Vec4 getPixelFloat(i32 X, i32 Y) const
        {
            const int offset = mChannels * ( Y * mWidth + X);
            const float* data = reinterpret_cast<const float*>(Data.data());
            return Vec4(
                mChannels > 0 ? data[offset + 0] : 0.0f,
                mChannels > 1 ? data[offset + 1] : 0.0f,
                mChannels > 2 ? data[offset + 2] : 0.0f,
                mChannels > 3 ? data[offset + 3] : 0.0f
            );
        }

        // glm::vec4 getPixelFloat(i32 X, i32 Y) const
        // {
        //     const int offset = mChannels * ( Y * mWidth + X);
        //     const float* data = reinterpret_cast<const float*>(Data.data());
        //     return glm::vec4(
        //         mChannels > 0 ? data[offset + 0] : 0.0f,
        //         mChannels > 1 ? data[offset + 1] : 0.0f,
        //         mChannels > 2 ? data[offset + 2] : 0.0f,
        //         mChannels > 3 ? data[offset + 3] : 0.0f
        //     );
        // }

        void setPixelUnsignedByte(i32 X, i32 Y, const Vec4& Value)
        {
            const i32 offset = mChannels * (Y * mWidth + X);
            if(mChannels > 0) Data[offset + 0] = u8(Value.X * 255.0f);
            if(mChannels > 1) Data[offset + 1] = u8(Value.Y * 255.0f);
            if(mChannels > 2) Data[offset + 2] = u8(Value.Z * 255.0f);
            if(mChannels > 3) Data[offset + 3] = u8(Value.W * 255.0f);
        }

        // void setPixelUnsignedByte(i32 X, i32 Y, const glm::vec4& Value)
        // {
        //     const i32 offset = mChannels * (Y * mWidth + X);
        //     if(mChannels > 0) Data[offset + 0] = u8(Value.x * 255.0f);
        //     if(mChannels > 1) Data[offset + 1] = u8(Value.y * 255.0f);
        //     if(mChannels > 2) Data[offset + 2] = u8(Value.z * 255.0f);
        //     if(mChannels > 3) Data[offset + 3] = u8(Value.w * 255.0f);
        // }

        Vec4 getPixelUnsignedByte(i32 X, i32 Y) const
        {
            const i32 offset = mChannels * (Y * mWidth + X);
            return Vec4(
                mChannels > 0 ? float(Data[offset + 0]) / 255.0f : 0.0f,
                mChannels > 1 ? float(Data[offset + 1]) / 255.0f : 0.0f,
                mChannels > 2 ? float(Data[offset + 2]) / 255.0f : 0.0f,
                mChannels > 3 ? float(Data[offset + 3]) / 255.0f : 0.0f
            );
        }

        // glm::vec4 getPixelUnsignedByte(i32 X, i32 Y) const
        // {
        //     const i32 offset = mChannels * (Y * mWidth + X);
        //     return glm::vec4(
        //         mChannels > 0 ? float(Data[offset + 0]) / 255.0f : 0.0f,
        //         mChannels > 1 ? float(Data[offset + 1]) / 255.0f : 0.0f,
        //         mChannels > 2 ? float(Data[offset + 2]) / 255.0f : 0.0f,
        //         mChannels > 3 ? float(Data[offset + 3]) / 255.0f : 0.0f
        //     );
        // }

    };

}
