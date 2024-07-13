#include "Rope.h"

namespace QP {

    void RopeParticle::applyForce(const Vec2& force) {
        acceleration += force;
    }

    void RopeParticle::update(float dt) {
        Vec2 velocity = position - prevPosition;
        prevPosition = position;
        position += velocity + acceleration * dt * dt;
        acceleration = Vec2(0, 0);
    }

    void RopeConstraint::satisfy() {
        for (int i = 0; i < 2500; ++i) {

        
            Vec2 delta = p2->position - p1->position;
            float deltaLength = delta.length();
            float diff = (deltaLength - restLength) / deltaLength;

            Vec2 correction = delta * 0.5f * diff;
            p1->position += correction;
            p2->position = p2->position - correction;
        }
    }


    void Rope::applyForce(const Vec2& force) {
        // Start from the second particle to apply force
        for (size_t i = 1; i < particles.size(); ++i) {
            particles[i].applyForce(force);
        }
    }

    void Rope::applyMouseForce(const Vec2& mousePos) {
        for (auto& particle : particles) {
            Vec2 force = mousePos - particle.position;
            particle.applyForce(force);
        }
    }

    void Rope::update(float dt) {

        Vec2 gravity(0, -9.81f);

        applyForce(gravity);

        for (auto& particle : particles) {
            particle.update(dt);
        }

        for (int i = 0; i < 5; ++i) {
            for (auto& constraint : constraints) {
                constraint.satisfy();
            }
        }
        // Ensure the first particle remains static
        if (!particles.empty()) {
            particles[0].position = particles[0].prevPosition;
        }
    }
}