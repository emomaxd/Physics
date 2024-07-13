#include "Pendulum.h"
#include <cmath>

namespace QP {

    Pendulum::Pendulum(const Vec3& initial_position, double length, double gravity)
        : position(initial_position), previous_position(initial_position), angular_velocity(0.0),
        angular_acceleration(0.0), length(length), gravity(gravity) {}

    void Pendulum::update(double dt) {
        calculateAngularAcceleration();
        angular_velocity += angular_acceleration * dt;
        position.y += angular_velocity * dt; // Simplified motion
    }

    Vec3 Pendulum::getPosition() const {
        return position;
    }

    void Pendulum::calculateAngularAcceleration() {
        angular_acceleration = -(gravity / length) * std::sin(position.y);
    }

}