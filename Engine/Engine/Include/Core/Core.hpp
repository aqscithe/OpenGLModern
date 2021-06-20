#pragma once

#include <cmath>
#include <string>

namespace Core
{
    struct rdrVertex
    {
        float x, y, z;    // Pos
        float r, g, b, a; // Color
        float nx, ny, nz; // Normal
        float u, v;       // Texture coordinates
    };

    namespace Debug
    {
        enum class LogType
        {
            DEBUG = 0,
            INFO = 1,
            WARNING = 2,
            ERROR = 3,
            FATAL = 4
        };

        enum class LogFile
        {
            MESSAGE = 0,
            ERROR = 1
        };

        class Assertion;
        class Log;
    }

    namespace Maths
    {
        #pragma region Union Declarations
        
        union vec2
        {
            float   e[2];
            //int     ei[2];
            struct { float x; float y; };
            //struct { int x; int y; };
        };

        union vec3
        {
            float   e[3];
            //int     ei[3];
            struct { float x; float y; float z; };
            struct { float r; float g; float b; };
            //struct { int x; int y; int z; };
            //struct { int r; int g; int b; };
            vec2 xy;
        };

        union vec4
        {
            vec4() = default;
            vec4(float x, float y, float z, float w)
                : x(x), y(y), z(z), w(w)
            {}

            vec4(vec3 xyz, float w)
                : x(xyz.x), y(xyz.y), z(xyz.z), w(w)
            {}

            float e[4];
            struct { float x; float y; float z; float w; };
            struct { float r; float g; float b; float a; };
            vec3 rgb;
            vec3 xyz;
            vec2 xy;
        };

        union mat4
        {
            float e[16];
            vec4 c[4];
        };
        
        #pragma endregion
         
        #pragma region vec3 
        inline vec3 operator+(vec3 a, vec3 b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }

        inline vec3 operator+=(vec3& a, vec3 b)
        {
            a = a + b;
            return a;
        }

        inline vec3 operator-(vec3 a) { return { -a.x, -a.y, -a.z }; }

        inline vec3 operator-(vec3 a, vec3 b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }

        inline vec3 operator/(vec3 v, float w) { return { v.x / w, v.y / w, v.z / w }; }

        inline vec3 operator*(vec3 v, float k) { return { v.x * k, v.y * k, v.z * k }; }
        inline vec3 operator*(float k, vec3 v) { return { v.x * k, v.y * k, v.z * k }; }
        inline vec3 operator*(vec3 a, vec3 b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }

        inline vec3 operator*=(vec3& a, float k)
        {
            a = a * k;
            return a;
        }

        inline vec3 operator*=(vec3& a, vec3 b)
        {
            a = a * b;
            return a;
        }
        
        #pragma endregion

        #pragma region vec4
        
        inline vec4 operator*(const mat4& m, vec4 v)
        {
            vec4 r;
            r.x = v.x * m.c[0].e[0] + v.y * m.c[1].e[0] + v.z * m.c[2].e[0] + v.w * m.c[3].e[0];
            r.y = v.x * m.c[0].e[1] + v.y * m.c[1].e[1] + v.z * m.c[2].e[1] + v.w * m.c[3].e[1];
            r.z = v.x * m.c[0].e[2] + v.y * m.c[1].e[2] + v.z * m.c[2].e[2] + v.w * m.c[3].e[2];
            r.w = v.x * m.c[0].e[3] + v.y * m.c[1].e[3] + v.z * m.c[2].e[3] + v.w * m.c[3].e[3];
            return r;
        }
        #pragma endregion   

        #pragma region mat4
        
        mat4 identity();
        mat4 translate(vec3 t);
        mat4 rotateX(float pitch);
        mat4 rotateY(float yaw);
        mat4 rotateZ(float roll);
        mat4 scale(float s);
        mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
        mat4 perspective(float fovY, float aspect, float near, float far, bool ortho);
        mat4 frustum(float left, float right, float bottom, float top, float near, float far);

        inline mat4 operator*(const mat4& a, const mat4& b)
        {
            mat4 res = {};
            for (int c = 0; c < 4; ++c)
                for (int r = 0; r < 4; ++r)
                    for (int i = 0; i < 4; ++i)
                        res.c[c].e[r] += a.c[i].e[r] * b.c[c].e[i];
            return res;
        }

        inline mat4& operator*=(mat4& a, const mat4& b)
        {
            a = a * b;
            return a;
        }
        #pragma endregion
        
        #pragma region Constants & common math functions

        const float TAU = 6.283185307179586476925f;

        template<typename T>
        inline int min(T x, T y) { return x < y ? x : y; };
        template<typename T>
        inline int max(T x, T y) { return x > y ? x : y; };

        inline float cos(float x) { return cosf(x); }
        inline float sin(float x) { return sinf(x); }
        inline float tan(float x) { return tanf(x); }

        inline float toRadians(float degrees) { return degrees * TAU / 360.f; };

        inline float dotProduct(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
        inline float magnitude(vec3 v) { return sqrtf(dotProduct(v, v)); }

        inline float max(float a, float b) { return a > b ? a : b; }
        inline float min(float a, float b) { return a < b ? a : b; }
        inline float clamp(float min, float max, float val) { return val < min ? min : val > max ? max : val; }

        vec3 normalize(vec3 v);
        bool invert(const float in[16], float out[16]);

        #pragma endregion
    }

}