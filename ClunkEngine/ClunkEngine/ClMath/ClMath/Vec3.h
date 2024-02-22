#pragma once


#include <PCH/pch.h>

#include <cmath>
#include <functional>

#include "HashHelper.h"

namespace Clunk
{
    template<typename T>
    class TVec3
    {
    public:
    
        // Anonymous union allows for accessing the Vec3 data in multiple ways
        union 
        {
            struct 
            {
                T X, Y, Z;
            };

            struct
            {
                T U, V, W;
            };

            struct
            {
                T R, G, B;
            };
            
            T Data[3];
        };       

        TVec3(const T X = T{}, const T Y = T{}, const T Z = T{}) : X(X), Y(Y), Z(Z) {};
        TVec3(const T Values[3])
        {
            X = Values[0];
            Y = Values[1];
            Z = Values[2];
        }

//////////////////////////////////////////////////////////

        T& operator[](u8 Index)
        {
            return ((&X)[Index]);
        }

        const T& operator[](u8 Index) const
        {
            return ((&x)[Index]);
        }

/////////////////////////////////////////////////////////

        // bool operator==(const TVec3& Other)
        // {
        //     return X == Other.X && Y == Other.Y && Z == Other.Z;
        // }

        const bool operator==(const TVec3& Other) const
        {
            return X == Other.X && Y == Other.Y && Z == Other.Z;
        }

        void operator=(const TVec3& Other)
        {
            X = Other.X;
            Y = Other.Y;
            Z = Other.Z;
        }

///////////////////////////////////////////////////////

        TVec3 operator+(const T Val)
        {
            TVec3 v;
            v = X + Val;
            v = Y + Val;
            v = Z + Val;
            return v;
        }

        const TVec3 operator+(const T Val) const
        {
            TVec3 v;
            v = X + Val;
            v = Y + Val;
            v = Z + Val;
            return v;
        }

        void operator+=(const T Val)
        {
            X += Val;
            Y += Val;
            Z += Val;
        }

        TVec3 operator+(const TVec3& Other)
        {
            TVec3 v;
            v.X = X + Other.X;
            v.Y = Y + Other.Y;
            v.Z = Z + Other.Z;
            return v;
        }

        const TVec3 operator+(const TVec3& Other) const
        {
            TVec3 v;
            v.X = X + Other.X;
            v.Y = Y + Other.Y;
            v.Z = Z + Other.Z;
            return v;
        }

        void operator+=(const TVec3& Other)
        {
            X += Other.X;
            Y += Other.Y;
            Z += Other.Z;
        }


///////////////////////////////////////////////////

        TVec3 operator-()
        {
            return TVec3(-X, -Y, -Z);
        }

        TVec3 operator-() const
        {
            return TVec3(-X, -Y, -Z);
        }

        TVec3 operator-(const T Val)
        {
            T v;
            v = X - Val;
            v = Y - Val;
            v = Z - Val;
            return v;
        }

        const TVec3 operator-(const T Val) const
        {
            TVec3 v;
            v = X - Val;
            v = Y - Val;
            v = Z - Val;
            return v;
        }

        void operator-=(const T Val)
        {
            X -= Val;
            Y -= Val;
            Z -= Val;
        }

        TVec3 operator-(const TVec3& Other)
        {
            TVec3 v;
            v.X = X - Other.X;
            v.Y = Y - Other.Y;
            v.Z = Z - Other.Z;
            return v;
        }

        const TVec3 operator-(const TVec3& Other) const
        {
            TVec3 v;
            v.X = X - Other.X;
            v.Y = Y - Other.Y;
            v.Z = Z - Other.Z;
            return v;
        }

        void operator-=(const TVec3& Other)
        {
            X -= other.X;
            Y -= other.Y;
            Z -= other.Z;
        }

//////////////////////////////////////////////

        TVec3 operator*(T Val)
        {
            TVec3 v;
            v.X = X * Val;
            v.Y = Y * Val;
            v.Z = Z * Val;
            return v;
        }

        const TVec3 operator*(T Val) const
        {
            TVec3 v;
            v.X = X * Val;
            v.Y = Y * Val;
            v.Z = Z * Val;
            return v;
        }

        void operator*=(T Val)
        {
            X *= Val;
            Y *= Val;
            Z *= Val;
        }

        TVec3 operator*(const TVec3& Other)
        {
            return TVec3(
                X * Other.X,
                Y * Other.Y,
                Z * Other.Z
            );
        }

        const TVec3 operator*(const TVec3& Other) const
        {
            return Vec3(
                X * Other.X,
                Y * Other.Y,
                Z * Other.Z
            );
        }

        void operator*=(const TVec3& Other)
        {
            X *= Other.X;
            Y *= Other.Y;
            Z *= Other.Z;
        }

    ///////////////////////////////////////////////

        void operator/=(T Val)
        {
            Val = T(1) / Val;
            X *= Val;
            Y *= Val;
            Z *= Val;
        }

        TVec3 operator/(T Val)
        {
            TVec3 v;
            Val = T(1) / Val;
            v.X = X * Val;
            v.Y = Y * Val;
            v.Z = Z * Val;
            // v.x = x / val;
            // v.y = y / val;
            // v.z = z / val;
            return v;
        }

        const TVec3 operator/(T Val) const
        {
            TVec3 v;
            Val = T(1) / Val;
            v.X = X * Val;
            v.Y = Y * Val;
            v.Z = Z * Val;
            // v.x = x / val;
            // v.y = y / val;
            // v.z = z / val;
            return v;
        }

//////////////////////////////////////////////

        /**
         * @brief Returns the Length of the vector.
         * If the exact length is not needed, then LengthSquared() is 
         * the more optimal choice to use.
         * @return Length of Vector
         */
        T Length()
        {
            return std::sqrt(X*X + Y*Y + Z*Z);
        }
        
        /**
         * @brief Returns the Length of the vector.
         * If the exact length is not needed, then LengthSquared() is 
         * the more optimal choice to use.
         * @return Length of Vector 
         */
        const T Length() const
        {
            return std::sqrt(X*X + Y*Y + Z*Z);
        }

        /**
         * @brief Returns the Length^2 of the vector. 
         * When comparing the length of 2 vectors, 
         * this is more optimal than Length() which
         * does an extra square root calculation.
         * @return Length of Vector squared. 
         */
        T LengthSquared()
        {
            return X*X + Y*Y + Z*Z;
        }

        /**
         * @brief Returns the Length^2 of the vector. 
         * When comparing the length of 2 vectors, 
         * this is more optimal than Length() which
         * does an extra square root calculation.
         * @return Length of Vector squared
         */
        const T LengthSquared() const
        {
            return X*X + Y*Y + Z*Z;
        }

        void Normalize()
        {
            // TODO: Test this
            *this /= Length();
        }

        TVec3 GetNormal()
        {
            // TODO: Test this
            return *this / Length();
        }

        static TVec3 Normalize(const TVec3& Vec)
        {
            return Vec / Vec.Length();
        }

        const TVec3 GetNormal() const
        {
            // TODO: Test this
            return *this / Length();
        }

        static TVec3 GetNormal(const TVec3 V)
        {
            return V / V.Length();
        }

        /**
         * @brief Dot product properties: 
         *  can be used to get the angle of 2 vectors,
         *  Is positive if vectors point in the same direction,
         *  Is negative if vectors point in the opposite direction, 
         *  Is zero if vectors are perpendicular.
         *  If both vectors have a unit length then Dot product has
         *  a range of -1 to 1.
         * @param Other TVec3
         * @return Dot product 
         */
        T Dot(const TVec3& Other)
        {
            return (X * Other.X + Y * Other.Y + Z * Other.Z);
        }

        /**
         * @brief Dot product properties: 
         *  can be used to get the angle of 2 vectors,
         *  Is positive if vectors point in the same direction,
         *  Is negative if vectors point in the opposite direction, 
         *  Is zero if vectors are perpendicular.
         *  If both vectors have a unit length then Dot product has
         *  a range of -1 to 1.
         * @param other TVec3
         * @return Dot product 
         */
        static T Dot(const TVec3& V1, const TVec3& V2)
        {
            return (V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z);
        }

        TVec3 Cross(const TVec3& Other)
        {
            TVec3 v(
                Y * Other.Z - Z * Other.Y,
                Z * Other.X - X * Other.Z,
                X * Other.Y - Y * Other.X
            );
            return v;
        }

        static TVec3 Cross(const TVec3& V1, const TVec3& V2)
        {
            TVec3 v(
                V1.Y * V2.Z - V1.Z * V2.Y,
                V1.Z * V2.X - V1.X * V2.Z,
                V1.X * V2.Y - V1.Y * V2.X
            );
            return v;
        }

        /**
         * @brief Returns the angle between to vectors in radians
         * @param V1 
         * @param V2 
         * @return f32 
         */
        static T Angle(const TVec3& V1, const TVec3& V2)
        {
            T sqV1 = V1.LengthSquared();
            T sqV2 = V2.LengthSquared();

            if(sqV1 < EPSILON || sqV2 < EPSILON) { return T{}; }
            T dot = TVec3::Dot(V1, V1);
            T length = sqrt(sqV1) * sqrt(sqV2);

            return cosf(dot/ length);
        }

        static TVec3 Project(const TVec3& V1, const TVec3& V2)
        {
            T lenV2 = V2.Length();
            if(lenV2 < EPSILON) { return TVec3(); }
            T scale = TVec3::Dot(V1, V2) / lenV2;
            return V2 * scale;
        }

        static TVec3 Reject(const TVec3& V1, const TVec3& V2)
        {
            TVec3 projection = TVec3::Project(V1, V2);
            return V1 - projection;
        }

        static TVec3 BounceReflect(const TVec3& V1, const TVec3& V2)
        {
            T lenV2 = V2.Length();
            if(lenV2 < EPSILON) { return TVec3(); }
            T scale = TVec3::Dot(V1, V2) / lenV2;
            TVec3 proj2 = V2 * (scale * 2);
            return V1 - proj2;
        }

        /**
         * @brief Linear Interpolation
         * @param Start 
         * @param End 
         * @param Time 
         * @return Vec3 
         */
        static TVec3 Lerp(const TVec3& Start, const TVec3& End, T Time)
        {
            return TVec3(
                Start.X + (End.X - Start.X) * Time,
                Start.Y + (End.Y - Start.Y) * Time,
                Start.Z + (End.Z - Start.Z) * Time
            );
        }

        /**
         * @brief Spherical Linear Interpolation. Shortest Arc.
         * @param Start 
         * @param End 
         * @param Time 
         * @return TVec3 
         */
        static TVec3 SLerp(const TVec3& Start, const TVec3& End, T Time)
        {
            if(Time < 0.01f) { return Lerp(Start, End, Time); }

            TVec3 from = Start.GetNormal();
            TVec3 to = End.GetNormal();

            T theta = TVec3::Angle(from, to);
            T sin_theta = sinf(theta);

            T a = sinf((T(1) - Time) * theta) / sin_theta;
            T b = sinf(Time * theta) / sin_theta;

            return from * a + to * b;
        }

        /**
         * @brief Normalized Linear Interpolation.
         * 
         * @param Start 
         * @param End 
         * @param Time 
         * @return Vec3 
         */
        static TVec3 NLerp(const TVec3& Start, const TVec3& End, T Time)
        {
            TVec3 V(
                Start.X + (End.X - Start.X) * Time,
                Start.Y + (End.Y - Start.y) * Time,
                Start.Z + (End.Z - Start.Z) * Time
            );
            return V.GetNormal();
        }
    };

//     class Vec3
//     {
//     public:
    
//         // Anonymous union allows for accessing the Vec3 data in multiple ways
//         union 
//         {
//             struct 
//             {
//                 f32 x, y, z;
//             };

//             struct
//             {
//                 f32 u, v, w;
//             };

//             struct
//             {
//                 f32 r, g, b;
//             };
            
//             f32 vec[3];
//         };       

//         Vec3(const f32 x = 0.0f, const f32 y = 0.0f, const f32 z = 0.0f) : x(x), y(y), z(z) {};
//         Vec3(const f32 values[3])
//         {
//             x = values[0];
//             y = values[1];
//             z = values[2];
//         }

// //////////////////////////////////////////////////////////

//         f32& operator[](u8 i)
//         {
//             return ((&x)[i]);
//         }

//         const f32& operator[](u8 i) const
//         {
//             return ((&x)[i]);
//         }

// /////////////////////////////////////////////////////////

//         // bool operator==(const Vec3& Other)
//         // {
//         //     return x == Other.x && y == Other.y && z == Other.z;
//         // }

//         const bool operator==(const Vec3& Other) const
//         {
//             return x == Other.x && y == Other.y && z == Other.z;
//         }

//         void operator=(const Vec3& Other)
//         {
//             x = Other.x;
//             y = Other.y;
//             z = Other.z;
//         }

// ///////////////////////////////////////////////////////

//         Vec3 operator+(const f32 val)
//         {
//             Vec3 v;
//             v = x + val;
//             v = y + val;
//             v = z + val;
//             return v;
//         }

//         const Vec3 operator+(const f32 val) const
//         {
//             Vec3 v;
//             v = x + val;
//             v = y + val;
//             v = z + val;
//             return v;
//         }

//         void operator+=(const f32 val)
//         {
//             x += val;
//             y += val;
//             z += val;
//         }

//         Vec3 operator+(const Vec3& other)
//         {
//             Vec3 v;
//             v.x = x + other.x;
//             v.y = y + other.y;
//             v.z = z + other.z;
//             return v;
//         }

//         const Vec3 operator+(const Vec3& other) const
//         {
//             Vec3 v;
//             v.x = x + other.x;
//             v.y = y + other.y;
//             v.z = z + other.z;
//             return v;
//         }

//         void operator+=(const Vec3& other)
//         {
//             x += other.x;
//             y += other.y;
//             z += other.z;
//         }


// ///////////////////////////////////////////////////

//         Vec3 operator-(const f32 val)
//         {
//             Vec3 v;
//             v = x - val;
//             v = y - val;
//             v = z - val;
//             return v;
//         }

//         const Vec3 operator-(const f32 val) const
//         {
//             Vec3 v;
//             v = x - val;
//             v = y - val;
//             v = z - val;
//             return v;
//         }

//         void operator-=(const f32 val)
//         {
//             x -= val;
//             y -= val;
//             z -= val;
//         }

//         Vec3 operator-(const Vec3& other)
//         {
//             Vec3 v;
//             v.x = x - other.x;
//             v.y = y - other.y;
//             v.z = z - other.z;
//             return v;
//         }

//         const Vec3 operator-(const Vec3& other) const
//         {
//             Vec3 v;
//             v.x = x - other.x;
//             v.y = y - other.y;
//             v.z = z - other.z;
//             return v;
//         }

//         void operator-=(const Vec3& other)
//         {
//             x -= other.x;
//             y -= other.y;
//             z -= other.z;
//         }

// //////////////////////////////////////////////

//         Vec3 operator*(f32 val)
//         {
//             Vec3 v;
//             v.x = x * val;
//             v.y = y * val;
//             v.z = z * val;
//             return v;
//         }

//         const Vec3 operator*(f32 val) const
//         {
//             Vec3 v;
//             v.x = x * val;
//             v.y = y * val;
//             v.z = z * val;
//             return v;
//         }

//         void operator*=(f32 val)
//         {
//             x *= val;
//             y *= val;
//             z *= val;
//         }

//         Vec3 operator*(const Vec3& Other)
//         {
//             return Vec3(
//                 x * Other.x,
//                 y * Other.y,
//                 z * Other.z
//             );
//         }

//         const Vec3 operator*(const Vec3& Other) const
//         {
//             return Vec3(
//                 x * Other.x,
//                 y * Other.y,
//                 z * Other.z
//             );
//         }

//         void operator*=(const Vec3& Other)
//         {
//             x *= Other.x;
//             y *= Other.y;
//             z *= Other.z;
//         }

//     ///////////////////////////////////////////////

//         void operator/=(f32 val)
//         {
//             val = 1.0f / val;
//             x *= val;
//             y *= val;
//             z *= val;
//         }

//         Vec3 operator/(f32 val)
//         {
//             Vec3 v;
//             val = 1.0f / val;
//             v.x = x * val;
//             v.y = y * val;
//             v.z = z * val;
//             // v.x = x / val;
//             // v.y = y / val;
//             // v.z = z / val;
//             return v;
//         }

//         const Vec3 operator/(f32 val) const
//         {
//             Vec3 v;
//             val = 1.0f / val;
//             v.x = x * val;
//             v.y = y * val;
//             v.z = z * val;
//             // v.x = x / val;
//             // v.y = y / val;
//             // v.z = z / val;
//             return v;
//         }

// //////////////////////////////////////////////

//         /**
//          * @brief Returns the Length of the vector.
//          * If the exact length is not needed, then LengthSquared() is 
//          * the more optimal choice to use.
//          * @return f32 
//          */
//         f32 Length()
//         {
//             return std::sqrt(x*x + y*y + z*z);
//         }
        
//         /**
//          * @brief Returns the Length of the vector.
//          * If the exact length is not needed, then LengthSquared() is 
//          * the more optimal choice to use.
//          * @return f32 
//          */
//         const f32 Length() const
//         {
//             return std::sqrt(x*x + y*y + z*z);
//         }

//         /**
//          * @brief Returns the Length^2 of the vector. 
//          * When comparing the length of 2 vectors, 
//          * this is more optimal than Length() which
//          * does an extra square root calculation.
//          * @return f32 
//          */
//         f32 LengthSquared()
//         {
//             return x*x + y*y + z*z;
//         }

//         /**
//          * @brief Returns the Length^2 of the vector. 
//          * When comparing the length of 2 vectors, 
//          * this is more optimal than Length() which
//          * does an extra square root calculation.
//          * @return f32 
//          */
//         const f32 LengthSquared() const
//         {
//             return x*x + y*y + z*z;
//         }

//         void Normalize()
//         {
//             // TODO: Test this
//             *this /= Length();
//         }

//         Vec3 GetNormal()
//         {
//             // TODO: Test this
//             return *this / Length();
//         }

//         static Vec3 Normalize(const Vec3& Vec)
//         {
//             return Vec / Vec.Length();
//         }

//         const Vec3 GetNormal() const
//         {
//             // TODO: Test this
//             return *this / Length();
//         }

//         static Vec3 GetNormal(const Vec3 V)
//         {
//             return V / V.Length();
//         }

//         /**
//          * @brief Dot product properties: 
//          *  can be used to get the angle of 2 vectors,
//          *  Is positive if vectors point in the same direction,
//          *  Is negative if vectors point in the opposite direction, 
//          *  Is zero if vectors are perpendicular.
//          *  If both vectors have a unit length then Dot product has
//          *  a range of -1 to 1.
//          * @param other Vec3
//          * @return f32 
//          */
//         f32 Dot(const Vec3& other)
//         {
//             return (x * other.x + y * other.y + z * other.z);
//         }

//         /**
//          * @brief Dot product properties: 
//          *  can be used to get the angle of 2 vectors,
//          *  Is positive if vectors point in the same direction,
//          *  Is negative if vectors point in the opposite direction, 
//          *  Is zero if vectors are perpendicular.
//          *  If both vectors have a unit length then Dot product has
//          *  a range of -1 to 1.
//          * @param other Vec3
//          * @return f32 
//          */
//         static f32 Dot(const Vec3& V1, const Vec3& V2)
//         {
//             return (V1.x * V2.x + V1.y * V2.y + V1.z * V2.z);
//         }

//         Vec3 Cross(const Vec3& other)
//         {
//             Vec3 v(
//                 y * other.z - z * other.y,
//                 z * other.x - x * other.z,
//                 x * other.y - y * other.x
//             );
//             return v;
//         }

//         static Vec3 Cross(const Vec3& V1, const Vec3& V2)
//         {
//             Vec3 v(
//                 V1.y * V2.z - V1.z * V2.y,
//                 V1.z * V2.x - V1.x * V2.z,
//                 V1.x * V2.y - V1.y * V2.x
//             );
//             return v;
//         }

//         /**
//          * @brief Returns the angle between to vectors in radians
//          * @param V1 
//          * @param V2 
//          * @return f32 
//          */
//         static f32 Angle(const Vec3& V1, const Vec3& V2)
//         {
//             f32 sqV1 = V1.LengthSquared();
//             f32 sqV2 = V2.LengthSquared();

//             if(sqV1 < EPSILON || sqV2 < EPSILON) { return 0.0f; }
//             f32 dot = Vec3::Dot(V1, V1);
//             f32 length = sqrt(sqV1) * sqrt(sqV2);

//             return cosf(dot/ length);
//         }

//         static Vec3 Project(const Vec3& V1, const Vec3& V2)
//         {
//             f32 lenV2 = V2.Length();
//             if(lenV2 < EPSILON) { return Vec3(); }
//             f32 scale = Vec3::Dot(V1, V2) / lenV2;
//             return V2 * scale;
//         }

//         static Vec3 Reject(const Vec3& V1, const Vec3& V2)
//         {
//             Vec3 projection = Vec3::Project(V1, V2);
//             return V1 - projection;
//         }

//         static Vec3 BounceReflect(const Vec3& V1, const Vec3& V2)
//         {
//             f32 lenV2 = V2.Length();
//             if(lenV2 < EPSILON) { return Vec3(); }
//             f32 scale = Vec3::Dot(V1, V2) / lenV2;
//             Vec3 proj2 = V2 * (scale * 2);
//             return V1 - proj2;
//         }

//         /**
//          * @brief Linear Interpolation
//          * @param Start 
//          * @param End 
//          * @param Time 
//          * @return Vec3 
//          */
//         static Vec3 Lerp(const Vec3& Start, const Vec3& End, f32 Time)
//         {
//             return Vec3(
//                 Start.x + (End.x - Start.x) * Time,
//                 Start.y + (End.y - Start.y) * Time,
//                 Start.z + (End.z - Start.z) * Time
//             );
//         }

//         /**
//          * @brief Spherical Linear Interpolation. Shortest Arc.
//          * @param Start 
//          * @param End 
//          * @param Time 
//          * @return Vec3 
//          */
//         static Vec3 SLerp(const Vec3& Start, const Vec3& End, f32 Time)
//         {
//             if(Time < 0.01f) { return Lerp(Start, End, Time); }

//             Vec3 from = Start.GetNormal();
//             Vec3 to = End.GetNormal();

//             f32 theta = Vec3::Angle(from, to);
//             f32 sin_theta = sinf(theta);

//             f32 a = sinf((1.0f - Time) * theta) / sin_theta;
//             f32 b = sinf(Time * theta) / sin_theta;

//             return from * a + to * b;
//         }

//         /**
//          * @brief Normalized Linear Interpolation.
//          * 
//          * @param Start 
//          * @param End 
//          * @param Time 
//          * @return Vec3 
//          */
//         static Vec3 NLerp(const Vec3& Start, const Vec3& End, f32 Time)
//         {
//             Vec3 V(
//                 Start.x + (End.x - Start.x) * Time,
//                 Start.y + (End.y - Start.y) * Time,
//                 Start.z + (End.z - Start.z) * Time
//             );
//             return V.GetNormal();
//         }
//     };
}

namespace std
{
    template<typename T> struct hash<Clunk::TVec3<T>>
    {
        size_t operator()(const Clunk::TVec3<T>& V) const
        {
            // size_t xHash = hash<f32>()(V.x);
            // size_t yHash = (hash<f32>()(V.y) << 1);
            // size_t zHash = (hash<f32>()(V.z) << 1);
            // return (xHash ^ yHash >> 1) ^ zHash;
            return ((hash<f32>()(V.X)
                    ^ (hash<f32>()(V.Y) << 1)) >> 1)
                    ^ (hash<f32>()(V.Z) << 1);
        }
    };
}

// void hash_combine(size_t &Seed, size_t Hash)
// {
//     Hash += 0x9e3779b9 + (Seed << 6) + (Seed >> 2);
//     Seed ^= Hash;
// }

namespace Clunk
{
    typedef TVec3<f32> Vec3;
    typedef TVec3<i32> IVec3;
}