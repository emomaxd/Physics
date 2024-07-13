#include "Cloth.h"
#include <iostream>
#include <cmath>

namespace QP {

    Particle::Particle(const Vec3& pos, bool is_pinned)
        : position(pos), old_position(pos), acceleration(0, 0, 0), pinned(is_pinned) {}

    Spring::Spring(int particle1, int particle2, double length)
        : p1(particle1), p2(particle2), rest_length(length) {}

    Cloth::Cloth(int width, int height, int num_particles_width, int num_particles_height)
        : num_particles_width(num_particles_width), num_particles_height(num_particles_height) 
    {
        particles.resize(num_particles_width * num_particles_height);
        double spacing_x = static_cast<double>(width) / (num_particles_width - 1);
        double spacing_y = static_cast<double>(height) / (num_particles_height - 1);

        for (int y = 0; y < num_particles_height; ++y) {
            for (int x = 0; x < num_particles_width; ++x) {
                int index = y * num_particles_width + x;
                particles[index] = Particle(Vec3(x * spacing_x, y * spacing_y, 0), y == 0 && (x % 3 == 0));
            }
        }

        for (int y = 0; y < num_particles_height; ++y) {
            for (int x = 0; x < num_particles_width; ++x) {
                if (x < num_particles_width - 1) {
                    springs.push_back(Spring(y * num_particles_width + x, y * num_particles_width + (x + 1), spacing_x));
                }
                if (y < num_particles_height - 1) {
                    springs.push_back(Spring(y * num_particles_width + x, (y + 1) * num_particles_width + x, spacing_y));
                }
            }
        }
    }

    void Cloth::applyForces() {
        for (auto& particle : particles) {
            if (!particle.pinned) {
                particle.acceleration.y += 9.81; // Gravity
            }
        }
    }

    void Cloth::satisfyConstraints() {
        for (const auto& spring : springs) {
            Particle& p1 = particles[spring.p1];
            Particle& p2 = particles[spring.p2];

            Vec3 direction = p2.position - p1.position;
            double distance = direction.length();
            double difference = spring.rest_length - distance;
            Vec3 offset = direction.normalized() * (difference / 2.0);

            if (!p1.pinned) {
                p1.position = p1.position - offset;
            }
            if (!p2.pinned) {
                p2.position = p2.position + offset;
            }
        }
    }

    void Cloth::verletIntegration(double dt) {
        for (auto& particle : particles) {
            if (!particle.pinned) {
                Vec3 temp_position = particle.position;

                particle.position = particle.position + (particle.position - particle.old_position) + particle.acceleration * dt * dt;
                particle.old_position = temp_position;

                particle.acceleration = Vec3(0, 0, 0);
            }
        }
    }

    void Cloth::update(double dt) {
        applyForces();
        satisfyConstraints();
        verletIntegration(dt);
    }

    void Cloth::draw() const {
        for (const auto& spring : springs) {
            const Particle& p1 = particles[spring.p1];
            const Particle& p2 = particles[spring.p2];
            std::cout << "Line from (" << p1.position.x << ", " << p1.position.y << ", " << p1.position.z
                << ") to (" << p2.position.x << ", " << p2.position.y << ", " << p2.position.z << ")\n";
        }
    }

}