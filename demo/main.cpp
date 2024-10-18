#include <iostream>
#include <thread>
#include <chrono>
#include "Nbody/nbody_cuda.cuh"


int main() {

    const size_t NUM_SPHERES = 1000;

    NBodySimulation nbody(NUM_SPHERES);

    while(true)
        nbody.update();

    return 0;
}
