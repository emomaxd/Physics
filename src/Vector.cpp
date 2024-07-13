#include "Vector.h"

namespace QP {

    Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3::Vec3(float xyz) : x(xyz), y(xyz), z(xyz) {}

    Vec3::Vec3(const Vec3& other) : x(other.x), y(other.y), z(other.z) {}

    Vec3::Vec3(Vec3&& other) noexcept : x(other.x), y(other.y), z(other.z) {
        other.x = 0.0f;
        other.y = 0.0f;
        other.z = 0.0f;
    }

    Vec3& Vec3::operator=(const Vec3& other) {
        if (this == &other) return *this;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Vec3& Vec3::operator=(Vec3&& other) noexcept {
        if (this == &other) return *this;
        x = other.x;
        y = other.y;
        z = other.z;
        other.x = 0.0f;
        other.y = 0.0f;
        other.z = 0.0f;
        return *this;
    }

    Vec3 Vec3::operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3& Vec3::operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3& Vec3::operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

	
    Vec3 Vec3::operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 Vec3::operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

    double Vec3::length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3 Vec3::normalized() const {
        double len = length();
        return len > 0 ? *this * (1.0 / len) : Vec3();
    }

    void normalize(Vec3& vec) {
        float len = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        if (len > 0.0f) {
            vec.x /= len;
            vec.y /= len;
            vec.z /= len;
        }
    }

    float dot(const Vec3& a, const Vec3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vec3 cross(const Vec3& a, const Vec3& b) {
        return Vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }


    Vec2::Vec2() : x(0), y(0) {}
    Vec2::Vec2(float x, float y) : x(x), y(y) {}

    Vec2 Vec2::operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 Vec2::operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 Vec2::operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    Vec2& Vec2::operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& Vec2::operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2& Vec2::operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    float Vec2::length() const {
        return std::sqrt(x * x + y * y);
    }

    Vec2 Vec2::normalized() const {
        float len = length();
        return Vec2(x / len, y / len);
    }


} // Namespace QP
