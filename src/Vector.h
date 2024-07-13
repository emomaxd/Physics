#pragma once

#include <iostream>
#include <cmath>

namespace QP {

    class Vec3 {
    public:
        float x, y, z;

        Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
        Vec3(float xyz);
        Vec3(const Vec3& other);
        Vec3(Vec3&& other) noexcept;
        ~Vec3() = default;

        Vec3& operator=(const Vec3& other);
        Vec3& operator=(Vec3&& other) noexcept;
        Vec3 operator+(const Vec3& other) const;
        Vec3& operator+=(const Vec3& other);
        Vec3 operator-(const Vec3& other) const;
        Vec3 operator*(float scalar) const;

        friend std::ostream& operator<<(std::ostream& os, const Vec3& vec);

        double length() const;
        Vec3 normalized() const;
    };

    void normalize(Vec3& vec);
    float dot(const Vec3& a, const Vec3& b);
    Vec3 cross(const Vec3& a, const Vec3& b);


    

    struct Vec2 {
        float x, y;

        Vec2();
        Vec2(float x, float y);

        Vec2 operator+(const Vec2& other) const;
        Vec2 operator-(const Vec2& other) const;
        Vec2 operator*(float scalar) const;
        Vec2& operator+=(const Vec2& other);
        Vec2& operator*=(float scalar);
        float length() const;
        Vec2 normalized() const;
    };


} // Namespace QP
