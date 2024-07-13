#pragma once

#include "Vector.h"

namespace QP {

    class Pendulum {
    public:
        Pendulum(const Vec3& initial_position, double length, double gravity);

        void update(double dt);

        Vec3 getPosition() const;

    private:
        Vec3 position;
        Vec3 previous_position;
        double angular_velocity;
        double angular_acceleration;
        double length;
        double gravity;

        void calculateAngularAcceleration();
    };

}