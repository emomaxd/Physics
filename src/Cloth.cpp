#include "Cloth.h"

namespace QP {

    
    // Particle implementation
    Particle::Particle(const Vec3& pos, float mass)
        : position(pos), previousPosition(pos), acceleration(0.0f, 0.0f, 0.0f), mass(mass), movable(true) {}

    void Particle::applyForce(const Vec3& force) {
        acceleration += force * (1.0f / mass);
    }

    void Particle::update(float deltaTime) {
        if (movable) {
            Vec3 temp = position;
            position += (position - previousPosition) + acceleration * (deltaTime * deltaTime);
            previousPosition = temp;
            acceleration = Vec3(0.0f, 0.0f, 0.0f);
        }
    }

    // Cloth implementation
    Cloth::Cloth(int width, int height, float restDistance, float particleMass)
        : width(width), height(height), restDistance(restDistance) {
        particles = new Particle * [height];
        for (int y = 0; y < height; ++y) {
            particles[y] = new Particle[width];
            for (int x = 0; x < width; ++x) {
                particles[y][x] = Particle(Vec3(x * restDistance, y * restDistance, 0.0f), particleMass);
            }
        }
        particles[0][0].movable = false;
        particles[0][width - 1].movable = false;
    }

    Cloth::~Cloth() {
        for (int y = 0; y < height; ++y) {
            delete[] particles[y];
        }
        delete[] particles;
    }

    void Cloth::applyGravity(const Vec3& gravity) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (particles[y][x].movable) {
                    particles[y][x].applyForce(gravity * particles[y][x].mass);
                }
            }
        }
    }

    void Cloth::applyWind(const Vec3& wind) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                particles[y][x].applyForce(wind);
            }
        }
    }

    void Cloth::update(float deltaTime) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                particles[y][x].update(deltaTime);
            }
        }
        satisfyConstraints();
    }

    void Cloth::addConstraint(int x1, int y1, int x2, int y2) {
        // This method is intentionally left empty for simplicity.
        // Implement constraints as per your requirements.
    }

    void Cloth::satisfyConstraints() {
        for (int y = 0; y < height - 1; ++y) {
            for (int x = 0; x < width - 1; ++x) {
                Vec3 diff = particles[y][x].position - particles[y + 1][x + 1].position;
                float currentDistance = diff.length();
                Vec3 correctionVector = diff * (1.0f - restDistance / currentDistance) * 0.5f;
                if (particles[y][x].movable) {
                    particles[y][x].position = particles[y][x].position - correctionVector;
                }
                if (particles[y + 1][x + 1].movable) {
                    particles[y + 1][x + 1].position += correctionVector;
                }
            }
        }
    }

} // namespace QP
