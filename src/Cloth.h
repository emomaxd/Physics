#pragma once

#include "Vector.h"
#include <vector>

namespace QP {

    struct Particle {
        Vec3 position;
        Vec3 old_position;
        Vec3 acceleration;
        bool pinned;

        Particle(const Vec3& pos, bool is_pinned);
        Particle() = default;
    };

    struct Spring {
        int p1, p2;
        double rest_length;

        Spring(int particle1, int particle2, double length);
    };

    class Cloth {
    public:
        Cloth(int width, int height, int num_particles_width, int num_particles_height);

        void update(double dt);
        void draw() const;

    private:
        int num_particles_width, num_particles_height;
        std::vector<Particle> particles;
        std::vector<Spring> springs;

        void applyForces();
        void satisfyConstraints();
        void verletIntegration(double dt);
    };

}