#pragma once

#include <iostream>
#include <cmath>
#include "Vector.h"

namespace QP {

    struct Pendulum {
        double length;
        double mass;
    };

    struct State {
        double theta1;
        double theta2;
        double omega1;
        double omega2;
    };

    struct Parameters {
        double g;
    };

    void applyMouseForce(const Pendulum& p1, const Pendulum& p2, State& state, const Parameters& params, const QP::Vec2& mousePos);
    void equations_of_motion(const Pendulum& p1, const Pendulum& p2, const Parameters& params, const State& state, State& deriv);
    void runge_kutta_step(const Pendulum& p1, const Pendulum& p2, const Parameters& params, State& state, double dt);

}