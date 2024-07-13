/// Verlet integration cloth simulation

#pragma once

#include <iostream>
#include <cmath>
#include <vector>

#include "Vector.h"
#include "Timestep.h"

namespace QP {



    class Particle {
    public: 
        Particle(float x, float y);

        void applyForce(const Vec2& force);
        void update(float ts);

    public:
        Vec2 position;
        Vec2 oldPosition;
        Vec2 acceleration;
        bool isStatic{false};

    };

    class Cloth {
    public:
        Cloth(size_t width, size_t height);
        
        void applyGravity();
        void update(float ts);

        void applyForce(const Vec2& force);
        void applyMouseForce(const Vec2& force);
        
        void resolveConstraint(int a, int b);
    
    public:
	std::vector<Particle> particles;
	std::vector<std::pair<int, int>> constraints;
    size_t width, height;
    };

} // namespace QP
