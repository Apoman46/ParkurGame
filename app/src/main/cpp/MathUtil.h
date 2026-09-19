#pragma once
#include <cmath>
#include <cstring>

// Küçük, bağımlılıksız 3D matematik yardımcıları (GLM yerine).
// Mat4 sütun-major (OpenGL uyumlu) float[16] dizisi olarak tutulur.

struct Vec3 {
    float x = 0, y = 0, z = 0;
    Vec3() {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator*(float s) const { return {x*s, y*s, z*s}; }
};

inline Vec3 normalize(const Vec3& v) {
    float len = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    if (len < 1e-6f) return {0,0,0};
    return {v.x/len, v.y/len, v.z/len};
}

inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
}

inline float dot(const Vec3& a, const Vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }

struct Mat4 {
    float m[16];

    static Mat4 identity() {
        Mat4 r{};
        std::memset(r.m, 0, sizeof(r.m));
        r.m[0] = r.m[5] = r.m[10] = r.m[15] = 1.0f;
        return r;
    }

    static Mat4 translate(const Vec3& t) {
        Mat4 r = identity();
        r.m[12] = t.x; r.m[13] = t.y; r.m[14] = t.z;
        return r;
    }

    static Mat4 scale(const Vec3& s) {
        Mat4 r = identity();
        r.m[0] = s.x; r.m[5] = s.y; r.m[10] = s.z;
        return r;
    }

    static Mat4 rotateY(float radians) {
        Mat4 r = identity();
        float c = std::cos(radians), s = std::sin(radians);
        r.m[0] = c;  r.m[2] = s;
        r.m[8] = -s; r.m[10] = c;
        return r;
    }

    // this * other (sütun-major çarpım)
    Mat4 multiply(const Mat4& o) const {
        Mat4 r{};
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                float sum = 0;
                for (int k = 0; k < 4; k++) sum += m[k*4+row] * o.m[col*4+k];
                r.m[col*4+row] = sum;
            }
        }
        return r;
    }

    static Mat4 perspective(float fovyRadians, float aspect, float nearZ, float farZ) {
        Mat4 r{};
        std::memset(r.m, 0, sizeof(r.m));
        float f = 1.0f / std::tan(fovyRadians / 2.0f);
        r.m[0] = f / aspect;
        r.m[5] = f;
        r.m[10] = (farZ + nearZ) / (nearZ - farZ);
        r.m[11] = -1.0f;
        r.m[14] = (2.0f * farZ * nearZ) / (nearZ - farZ);
        return r;
    }

    static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
        Vec3 f = normalize(center - eye);
        Vec3 s = normalize(cross(f, up));
        Vec3 u = cross(s, f);

        Mat4 r = identity();
        r.m[0] = s.x; r.m[4] = s.y; r.m[8]  = s.z;
        r.m[1] = u.x; r.m[5] = u.y; r.m[9]  = u.z;
        r.m[2] = -f.x; r.m[6] = -f.y; r.m[10] = -f.z;
        r.m[12] = -dot(s, eye);
        r.m[13] = -dot(u, eye);
        r.m[14] = dot(f, eye);
        return r;
    }
};

// Sadece Y ekseni etrafında dönüş içeren nesneler için normal matrisi (3x3),
// eksenle hizalı kutularda ölçeklendirme normal yönünü bozmaz.
struct Mat3 {
    float m[9];
    static Mat3 rotateY(float radians) {
        Mat3 r{};
        float c = std::cos(radians), s = std::sin(radians);
        // sütun-major 3x3
        r.m[0]=c; r.m[1]=0; r.m[2]=-s;
        r.m[3]=0; r.m[4]=1; r.m[5]=0;
        r.m[6]=s; r.m[7]=0; r.m[8]=c;
        return r;
    }
};
