#pragma once

#include <iostream>
#include <cmath>

#include "Vector.h"

namespace QP {



    class Particle {
    public:
        Vec3 position;
        Vec3 previousPosition;
        Vec3 acceleration;
        float mass;
        bool movable;

        Particle(const Vec3& pos, float mass);
        Particle() = default;

        void applyForce(const Vec3& force);
        void update(float deltaTime);
    };

    class Cloth {
    public:
        Particle** particles;
        int width;
        int height;
        float restDistance;

        Cloth(int width, int height, float restDistance, float particleMass);
        ~Cloth();

        void applyGravity(const Vec3& gravity);
        void applyWind(const Vec3& wind);
        void update(float deltaTime);
        void addConstraint(int x1, int y1, int x2, int y2);
        void satisfyConstraints();
    };

} // namespace QP
