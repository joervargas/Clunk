#include "ClCubemapUtils.h"

#include <cstdio>
#include <STB_Image/stb_image.h>

#include <Core/Logger.h>
#include <ClMath/Vec2.h>
#include <ClMath/Vec3.h>


namespace Clunk
{
    /// From Henry J. Warren's "Hacker's Delight"
    float radicalInverse_VdC(uint32_t bits)
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10f; // / 0x100000000
    }

    // The i-th point is then computed by

    /// From http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html

    Vec2 hammersley2d(uint32_t i, uint32_t N)
    {
        return Vec2(float(i)/float(N), radicalInverse_VdC(i));
    }

    Vec3 faceCoordsToXYZ(int I, int J, int FaceID, int FaceSize)
    {
        const f32 A = 2.0f * f32(I) / FaceSize;
        const f32 B = 2.0f * f32(J) / FaceSize;

        if(FaceID == 0) return Vec3(-1.0f, A - 1.0f, B - 1.0f);
        if(FaceID == 1) return Vec3(A - 1.0f, -1.0f, 1.0f - B);
        if(FaceID == 2) return Vec3(1.0f, A - 1.0f, 1.0f -B);
        if(FaceID == 3) return Vec3(1.0f - A, 1.0f, 1.0f - B);
        if(FaceID == 4) return Vec3(B - 1.0f, A - 1.0f, 1.0f);
        if(FaceID == 5) return Vec3(1.0f - B, A - 1.0f, -1.0f);
        return Vec3();
    }

    ClBitmap convertEquirectangularMapToVerticalCross(const ClBitmap &b)
    {
        if(b.GetType() == EBitmapType::EBitmapType_2D) return ClBitmap();

        const i32 faceSize = b.GetWidth() / 4;

        const i32 w = faceSize * 3;
        const i32 h = faceSize * 4;

        ClBitmap result(w, h, b.GetChannels(), b.GetFormat());
        const clOffset2D<i32> kFaceOffsets[] = 
        {
            clOffset2D<i32>(faceSize, faceSize * 3),
            clOffset2D<i32>(0, faceSize),
            clOffset2D<i32>(faceSize, faceSize),
            clOffset2D<i32>(faceSize, 2 * faceSize),
            clOffset2D<i32>(faceSize, 0),
            clOffset2D<i32>(faceSize, faceSize * 2)
        };

        const i32 clampW = b.GetWidth() - 1;
        const i32 clampH = b.GetHeight() - 1;

        for(i32 face = 0; face != 6; face++)
        {
            for(i32 i = 0; i != faceSize; i++)
            {
                for(i32 j = 0; j != faceSize; j++)
                {
                    const Vec3 P = faceCoordsToXYZ(i, j, face, faceSize);
                    const float R = hypot(P.x, P.y);
                    const float theta = atan2(P.y, P.x);
                    const float phi = atan2(P.z, R);
                    // float point source coordinates
                    const float Uf = float(2.0f * faceSize * (theta + Math::PI) / Math::PI);
                    const float Vf = float(2.0f * faceSize * (Math::PI / 2.0f - phi) / Math::PI);
                    // 4 samples for bilinear interpolation
                    const i32 U1 = std::clamp(i32(floor(Uf)), 0, clampW);
                    const i32 V1 = std::clamp(i32(floor(Vf)), 0, clampH);
                    const i32 U2 = std::clamp(U1 + 1, 0, clampW);
                    const i32 V2 = std::clamp(V1 + 1, 0, clampH);
                    // fractional part
                    const float s = Uf - U1;
                    const float t = Vf - V1;
                    // fetch 4-samples
                    const Vec4 A = b.GetPixel(U1, V1);
                    const Vec4 B = b.GetPixel(U2, V1);
                    const Vec4 C = b.GetPixel(U1, V2);
                    const Vec4 D = b.GetPixel(U2, V2);
                    // bilinear interpolation
                    const Vec4 color =  A * (1.0f - s) + 
                                        B * (s) * (1.0f - t) + 
                                        C * (1.0f - s) * (t) + 
                                        D * (s) * (t);
                    result.SetPixel(i + kFaceOffsets[face].X, j + kFaceOffsets[face].Y, color);
                } // j++
            } // i++
        } // face++

        return result;
    }

    ClBitmap convertVerticalCrossToCubeMapFaces(const ClBitmap &b)
    {
        const i32 face_width = b.GetWidth() / 3;
        const i32 face_height = b.GetHeight() / 4;

        ClBitmap cubemap(face_width, face_height, 6, b.GetChannels(), b.GetFormat());
        cubemap.SetType(EBitmapType::EBitmapType_Cube);

        const u8* src = b.Data.data();
        u8* dst = cubemap.Data.data();

        /*
            ------
            | +Y |
        ----------------
        | -X | -Z | +X |
        ----------------
            | -Y |
            ------
            | +Z |
            ------
        */

       const i32 pixel_size = cubemap.GetChannels() * ClBitmap::GetBytesPerComponent(cubemap.GetFormat());

       for(i32 face = 0; face != 6; ++face)
       {
            for(i32 j = 0; j != face_height; ++j)
            {
                for(i32 i = 0; i != face_width; ++i)
                {
                    i32 x = 0;
                    i32 y = 0;

                    switch(face)
                    {
                        // GL_TEXTURE_CUBE_MAP_POSITIVE_X
                        case 0:
                            x = i;
                            y = face_height + j;
                            break;
                        // GL_TEXTURE_CUBE_MAP_NEGATIVE_X
                        case 1:
                            x = 2 * face_width + i;
                            y = 1 * face_height + j;
                            break;
                        // GL_TEXTURE_CUBE_MAP_POSITIVE_Y
                        case 2:
                            x = 2 * face_width - (i + 1);
                            y = 1 * face_height - (j + 1);
                            break;
                        // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
                        case 3:
                            x = 2 * face_width - (i + 1);
                            y = 3 * face_height - (j + 1);
                            break;
                        // GL_TEXTURE_CUBE_MAP_POSITIVE_Z
                        case 4:
                            x = 2 * face_width - (i + 1);
                            y = b.GetHeight() - (j + 1);
                            break;
                        // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
                        case 5:
                            x = face_width + i;
                            y = face_height + j;
                            break;
                    }

                    memcpy(dst, src + (y * b.GetWidth() + x) * pixel_size, pixel_size);
                    dst += pixel_size;
                } // i
            } // j
       } // face

        return cubemap;
    }

    ClBitmap convertMultiFileToCubeMapFaces(std::vector<const char *> Files,  int* pWidth, int* pHeight)
    {
        if (Files.size() != 6) { return ClBitmap(); }
        int width = 0;
        int height = 0;
        int channels = 0;

        std::vector<u8> img_data;
        for(const char* file_path : Files)
        {
            stbi_uc* pixels = stbi_load(file_path, &width, &height, &channels, STBI_rgb_alpha);
            if(!pixels)
            {
                THROW_EXCEPTION("Failed to load cubemap image [%s]\n", file_path);
                fflush(stdout);
            }

            i32 img_size = width * height * 4;
            // Reserve an appropriate ammount in vector for optimization purposes.
            if(img_data.capacity() != (img_size * 6)) { img_data.reserve(img_size * 6); }

            for(int i = 0; i < img_size; i++)
            {
                img_data.push_back(pixels[i]);
            }
        }
        
        ClBitmap result(width, height, 6, channels, EBitmapFormat_UnsignedByte, img_data.data());
        if(pWidth) { *pWidth = width; }
        if(pHeight) { *pHeight = height; }
        
        return result;
    }
}
