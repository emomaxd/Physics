#include "nbody_cuda.cuh"
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include <algorithm>

#define BOX_SIZE 100.0f  // Box dimension
#define TIME_STEP 0.01f  // Time step for simulation

volatile sig_atomic_t stop_simulation = 0;

__global__ void update_positions(Sphere *spheres, int num_spheres, float dt) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= num_spheres) return;

    Sphere &s = spheres[idx];
    
    // Update position based on velocity
    s.position.x += s.velocity.x * dt;
    s.position.y += s.velocity.y * dt;
    s.position.z += s.velocity.z * dt;

    // Handle collisions with box boundaries
    if (s.position.x - s.radius < 0.0f || s.position.x + s.radius > BOX_SIZE) {
        s.velocity.x *= -1.0f;
        s.position.x = fminf(fmaxf(s.position.x, s.radius), BOX_SIZE - s.radius);
    }
    if (s.position.y - s.radius < 0.0f || s.position.y + s.radius > BOX_SIZE) {
        s.velocity.y *= -1.0f;
        s.position.y = fminf(fmaxf(s.position.y, s.radius), BOX_SIZE - s.radius);
    }
    if (s.position.z - s.radius < 0.0f || s.position.z + s.radius > BOX_SIZE) {
        s.velocity.z *= -1.0f;
        s.position.z = fminf(fmaxf(s.position.z, s.radius), BOX_SIZE - s.radius);
    }
}

__global__ void resolve_collisions(Sphere *spheres, int num_spheres) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= num_spheres) return;

    Sphere &s1 = spheres[idx];

    for (int j = idx + 1; j < num_spheres; j++) {
        Sphere &s2 = spheres[j];

        // Calculate the vector between the two spheres
        float3 diff = make_float3(s2.position.x - s1.position.x,
                                  s2.position.y - s1.position.y,
                                  s2.position.z - s1.position.z);

        // Distance between spheres
        float dist2 = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
        float min_dist = s1.radius + s2.radius;

        // If they are colliding
        if (dist2 < min_dist * min_dist) {
            float dist = sqrtf(dist2);
            float overlap = 0.5f * (min_dist - dist);

            // Normalize the difference vector
            diff.x /= dist;
            diff.y /= dist;
            diff.z /= dist;

            // Move the spheres apart based on the overlap
            s1.position.x -= overlap * diff.x;
            s1.position.y -= overlap * diff.y;
            s1.position.z -= overlap * diff.z;

            s2.position.x += overlap * diff.x;
            s2.position.y += overlap * diff.y;
            s2.position.z += overlap * diff.z;

            // Swap velocities in the direction of collision
            float3 velocity_diff = make_float3(s2.velocity.x - s1.velocity.x,
                                               s2.velocity.y - s1.velocity.y,
                                               s2.velocity.z - s1.velocity.z);
            float velocity_dot = velocity_diff.x * diff.x + velocity_diff.y * diff.y + velocity_diff.z * diff.z;

            if (velocity_dot > 0.0f) {
                s1.velocity.x += velocity_dot * diff.x;
                s1.velocity.y += velocity_dot * diff.y;
                s1.velocity.z += velocity_dot * diff.z;

                s2.velocity.x -= velocity_dot * diff.x;
                s2.velocity.y -= velocity_dot * diff.y;
                s2.velocity.z -= velocity_dot * diff.z;
            }
        }
    }
}

// Utility function to check for CUDA errors
cudaError_t NBodySimulation::checkCuda(cudaError_t result) {
    if (result != cudaSuccess) {
        fprintf(stderr, "CUDA Runtime Error: %s\n", cudaGetErrorString(result));
        exit(-1);
    }
    return result;
}

// Constructor
NBodySimulation::NBodySimulation(int num_spheres) : num_spheres(num_spheres) {
    h_spheres.resize(num_spheres);
    initializeSpheres();

    checkCuda(cudaMalloc(&d_spheres, num_spheres * sizeof(Sphere)));

    checkCuda(cudaMemcpy(d_spheres, h_spheres.data(), num_spheres * sizeof(Sphere), cudaMemcpyHostToDevice));
}

// Destructor
NBodySimulation::~NBodySimulation() {
    checkCuda(cudaFree(d_spheres));
}

void NBodySimulation::initializeSpheres() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> pos_dist(0.0f, BOX_SIZE);
    std::uniform_real_distribution<float> vel_dist(-5.0f, 5.0f);

    auto generate_sphere = [&]() {
        return Sphere{
            make_float3(pos_dist(gen), pos_dist(gen), pos_dist(gen)),
            make_float3(vel_dist(gen), vel_dist(gen), vel_dist(gen)),
            1.0f
        };
    };

    std::generate(h_spheres.begin(), h_spheres.end(), generate_sphere);
}

void NBodySimulation::update() {
    // Define the number of threads and blocks
    int threadsPerBlock = 256;
    int blocksPerGrid = (num_spheres + threadsPerBlock - 1) / threadsPerBlock;

    update_positions<<<blocksPerGrid, threadsPerBlock>>>(d_spheres, num_spheres, TIME_STEP);
    checkCuda(cudaDeviceSynchronize());

    resolve_collisions<<<blocksPerGrid, threadsPerBlock>>>(d_spheres, num_spheres);
    checkCuda(cudaDeviceSynchronize());

    checkCuda(cudaMemcpy(h_spheres.data(), d_spheres, num_spheres * sizeof(Sphere), cudaMemcpyDeviceToHost));
}

// Accessor for spheres
const std::vector<Sphere>& NBodySimulation::getSpheres() const {
    return h_spheres;
}