#include "Fluid.h"
#include <iostream>
#include <thread>
#include <chrono>

// Simple function to visualize the smoke (for demonstration purposes)
void visualize(const FluidSimulation& fluid) {
    const auto& grid = fluid.getGrid();
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 40; ++x) {
            if (grid[x][y].obstacle) {
                std::cout << "X";
            } else {
                float smoke = grid[x][y].smoke;
                if (smoke > 0.5f) {
                    std::cout << "#";
                } else if (smoke > 0.1f) {
                    std::cout << "+";
                } else {
                    std::cout << ".";
                }
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    FluidSimulation fluid(80, 40);

    int frame = 0;
    while (true) {
        float dt = 0.1f;

        
        
        fluid.addSmoke(20, 10, 0.1f);
        fluid.addVelocity(20, 10, 0.1f, 0.0f);

        fluid.update(dt);

        visualize(fluid);
        std::cout << "Frame: " << frame++ << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
