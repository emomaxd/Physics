#pragma once

#include <iostream>
#include <vector>
#include <cmath>

struct Vec2 {
    double x, y;
};

static Vec2 operator+(const Vec2& a, const Vec2& b) {
    return {a.x + b.x, a.y + b.y};
}

static Vec2 operator-(const Vec2& a, const Vec2& b) {
    return {a.x - b.x, a.y - b.y};
}

static Vec2 operator*(const Vec2& a, double b) {
    return {a.x * b, a.y * b};
}

static Vec2 operator/(const Vec2& a, double b) {
    return {a.x / b, a.y / b};
}

static double length(const Vec2& a) {
    return std::sqrt(a.x * a.y + a.y * a.y);
}

static Vec2 normalize(const Vec2& a) {
    double len = length(a);
    return {a.x / len, a.y / len};
}


struct Particle {
    Vec2 position;
    Vec2 prev_position;
    Vec2 acceleration;
    bool is_fixed;
};

struct Spring {
    int p1, p2;
    double rest_length;
};

struct Cloth {
    std::vector<Particle> particles;
    std::vector<Spring> springs;
    double width;
    double height;
    int num_particles_width;
    int num_particles_height;
    double stiffness;
    double damping;
};


static void initialize_cloth(Cloth& cloth, double width, double height, int num_particles_width, int num_particles_height) {
    cloth.width = width;
    cloth.height = height;
    cloth.num_particles_width = num_particles_width;
    cloth.num_particles_height = num_particles_height;
    cloth.stiffness = 1.0;
    cloth.damping = 0.01;

    double rest_length_x = width / (num_particles_width - 1);
    double rest_length_y = height / (num_particles_height - 1);

    // Create particles
    for (int j = 0; j < num_particles_height; ++j) {
        for (int i = 0; i < num_particles_width; ++i) {
            Particle p;
            p.position = {i * rest_length_x, j * rest_length_y};
            p.prev_position = p.position;
            p.acceleration = {0.0, 0.0};
            p.is_fixed = (j == 0); // Fix top row of particles
            cloth.particles.push_back(p);
        }
    }

    // Create springs
    for (int j = 0; j < num_particles_height; ++j) {
        for (int i = 0; i < num_particles_width; ++i) {
            if (i < num_particles_width - 1) {
                Spring s = {j * num_particles_width + i, j * num_particles_width + i + 1, rest_length_x};
                cloth.springs.push_back(s);
            }
            if (j < num_particles_height - 1) {
                Spring s = {j * num_particles_width + i, (j + 1) * num_particles_width + i, rest_length_y};
                cloth.springs.push_back(s);
            }
            if (i < num_particles_width - 1 && j < num_particles_height - 1) {
                Spring s1 = {j * num_particles_width + i, (j + 1) * num_particles_width + i + 1, std::sqrt(rest_length_x * rest_length_x + rest_length_y * rest_length_y)};
                Spring s2 = {(j + 1) * num_particles_width + i, j * num_particles_width + i + 1, std::sqrt(rest_length_x * rest_length_x + rest_length_y * rest_length_y)};
                cloth.springs.push_back(s1);
                cloth.springs.push_back(s2);
            }
        }
    }
}


static void apply_forces(Cloth& cloth, double dt) {
    Vec2 gravity = {0.0, -9.81};

    for (auto& particle : cloth.particles) {
        if (!particle.is_fixed) {
            particle.acceleration = gravity;
        }
    }
}

static void integrate(Cloth& cloth, double dt) {
    for (auto& particle : cloth.particles) {
        if (!particle.is_fixed) {
            Vec2 temp = particle.position;
            Vec2 velocity = (particle.position - particle.prev_position) * (1.0 - cloth.damping);
            particle.position = particle.position + velocity + particle.acceleration * (dt * dt);
            particle.prev_position = temp;
        }
    }
}

static void satisfy_constraints(Cloth& cloth) {
    for (auto& spring : cloth.springs) {
        Particle& p1 = cloth.particles[spring.p1];
        Particle& p2 = cloth.particles[spring.p2];

        Vec2 delta = p2.position - p1.position;
        double delta_length = length(delta);
        double difference = (delta_length - spring.rest_length) / delta_length;

        if (!p1.is_fixed) {
            p1.position = p1.position + delta * 0.5 * difference * cloth.stiffness;
        }
        if (!p2.is_fixed) {
            p2.position = p2.position - delta * 0.5 * difference * cloth.stiffness;
        }
    }
}

static void update_cloth(Cloth& cloth, double dt) {
    apply_forces(cloth, dt);
    integrate(cloth, dt);
    for (int i = 0; i < 5; ++i) { // Multiple iterations for constraint satisfaction
        satisfy_constraints(cloth);
    }
}
