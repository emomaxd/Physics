#pragma once
#include <vector>
#include "Vector.h"

namespace QP {


    class RopeParticle {
    public:
        RopeParticle(const Vec2& pos) : position(pos), prevPosition(pos), acceleration(0, 0) {}

        void applyForce(const Vec2& force);

        void update(float dt);


    public:
        Vec2 position;
        Vec2 prevPosition;
        Vec2 acceleration;
    };


    class RopeConstraint {
    public:
        RopeConstraint(RopeParticle* p1, RopeParticle* p2)
            : p1(p1), p2(p2), restLength((p2->position - p1->position).length()) {}

        void satisfy();

    public:
        RopeParticle* p1;
        RopeParticle* p2;
        float restLength;
    };


    class Rope {
    public:

        Rope(const Vec2& start, const Vec2& end, int segments) {
            Vec2 delta = (end - start) * (1.0f / (segments - 1));

            for (int i = 0; i < segments; ++i) {
                particles.emplace_back(start + delta * i);
            }

            for (int i = 0; i < segments - 1; ++i) {
                constraints.emplace_back(&particles[i], &particles[i + 1]);
            }
        }

        void applyForce(const Vec2& force);

        void Rope::applyMouseForce(const Vec2& mousePos);

        void update(float dt);
    
    public:
        std::vector<RopeParticle> particles;
        std::vector<RopeConstraint> constraints;
        
    };
}