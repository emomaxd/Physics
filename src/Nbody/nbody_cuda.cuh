#ifndef NBODY_SIMULATION_H
#define NBODY_SIMULATION_H

#include <vector>
#include <random>
#include <cuda_runtime.h>

// Sphere structure to store position, velocity, and radius
struct Sphere {
    float3 position;
    float3 velocity;
    float radius;
};

class NBodySimulation {
public:
    NBodySimulation(int num_spheres);
    ~NBodySimulation();
    
    void update();
    const std::vector<Sphere>& getSpheres() const;

private:
    Sphere* d_spheres;          // Device pointer for spheres
    std::vector<Sphere> h_spheres; // Host vector for spheres
    int num_spheres;

    cudaError_t checkCuda(cudaError_t result);
    void initializeSpheres();
};
#endif // NBODY_SIMULATION_H
