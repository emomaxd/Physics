#include "Pendulum.h"

namespace QP {

    void applyMouseForce(const Pendulum& p1, const Pendulum& p2, State& state, const Parameters& params, const QP::Vec2& mousePos) {
        double g = params.g;

        double theta1 = state.theta1;
        double theta2 = state.theta2;
        double delta_theta = theta1 - theta2;
        double L1 = p1.length;
        double L2 = p2.length;
        double m1 = p1.mass;
        double m2 = p2.mass;

        double denom1 = (2 * m1 + m2 - m2 * cos(2 * delta_theta));
        double denom2 = (L2 / L1) * denom1;

        double force1 = (m2 * L1 * state.omega1 * state.omega1 * sin(2 * delta_theta) + 2 * m1 * L1 * state.omega1 * state.omega1 * sin(delta_theta) + (m1 + m2) * g * sin(theta1)) / (L1 * denom1);
        double force2 = (-m2 * L2 * state.omega2 * state.omega2 * sin(2 * delta_theta) + (m1 + m2) * g * sin(theta2) + 2 * (m1 + m2) * L1 * state.omega1 * state.omega1 * sin(delta_theta)) / (L2 * denom2);

        state.omega1 += force1 * 0.005f;
        state.omega2 += force2 * 0.005f;
    }

    void equations_of_motion(const Pendulum& p1, const Pendulum& p2, const Parameters& params, const State& state, State& deriv) {
        double g = params.g;

        double m1 = p1.mass;
        double m2 = p2.mass;
        double L1 = p1.length;
        double L2 = p2.length;

        double theta1 = state.theta1;
        double theta2 = state.theta2;
        double omega1 = state.omega1;
        double omega2 = state.omega2;

        double delta_theta = theta1 - theta2;
        double denom1 = (2 * m1 + m2 - m2 * cos(2 * delta_theta));
        double denom2 = (L2 / L1) * denom1;

        deriv.theta1 = omega1;
        deriv.theta2 = omega2;

        deriv.omega1 = (-g * (2 * m1 + m2) * sin(theta1) - m2 * g * sin(theta1 - 2 * theta2) - 2 * sin(delta_theta) * m2 * (omega2 * omega2 * L2 + omega1 * omega1 * L1 * cos(delta_theta))) / (L1 * denom1);
        deriv.omega2 = (2 * sin(delta_theta) * (omega1 * omega1 * L1 * (m1 + m2) + g * (m1 + m2) * cos(theta1) + omega2 * omega2 * L2 * m2 * cos(delta_theta))) / (L2 * denom2);
    }

    void runge_kutta_step(const Pendulum& p1, const Pendulum& p2, const Parameters& params, State& state, double dt) {
        State k1, k2, k3, k4, temp_state;

        equations_of_motion(p1, p2, params, state, k1);

        temp_state = state;
        temp_state.theta1 += 0.5 * dt * k1.theta1;
        temp_state.theta2 += 0.5 * dt * k1.theta2;
        temp_state.omega1 += 0.5 * dt * k1.omega1;
        temp_state.omega2 += 0.5 * dt * k1.omega2;
        equations_of_motion(p1, p2, params, temp_state, k2);

        temp_state = state;
        temp_state.theta1 += 0.5 * dt * k2.theta1;
        temp_state.theta2 += 0.5 * dt * k2.theta2;
        temp_state.omega1 += 0.5 * dt * k2.omega1;
        temp_state.omega2 += 0.5 * dt * k2.omega2;
        equations_of_motion(p1, p2, params, temp_state, k3);

        temp_state = state;
        temp_state.theta1 += dt * k3.theta1;
        temp_state.theta2 += dt * k3.theta2;
        temp_state.omega1 += dt * k3.omega1;
        temp_state.omega2 += dt * k3.omega2;
        equations_of_motion(p1, p2, params, temp_state, k4);

        state.theta1 += (dt / 6.0) * (k1.theta1 + 2 * k2.theta1 + 2 * k3.theta1 + k4.theta1);
        state.theta2 += (dt / 6.0) * (k1.theta2 + 2 * k2.theta2 + 2 * k3.theta2 + k4.theta2);
        state.omega1 += (dt / 6.0) * (k1.omega1 + 2 * k2.omega1 + 2 * k3.omega1 + k4.omega1);
        state.omega2 += (dt / 6.0) * (k1.omega2 + 2 * k2.omega2 + 2 * k3.omega2 + k4.omega2);
    }
}
