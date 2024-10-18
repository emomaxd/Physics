#pragma once

#include <vector>

namespace QP {

    struct Velocity {
        float u; // velocity in x-direction
        float v; // velocity in y-direction
    };

    struct FluidCell {
        float pressure;
        float smoke;
        Velocity velocity;
        bool obstacle;
    };

    class FluidSimulation {
    public:
        FluidSimulation(int width, int height);
        void initializeGrid();
        void update(float dt);
        void addSmoke(int x, int y, float amount);
        void addVelocity(int x, int y, float u, float v);
        const std::vector<std::vector<FluidCell>>& getGrid() const;

    public:
        int width;
        int height;
        std::vector<std::vector<FluidCell>> grid;

        void advect(float dt);
        void diffuse(float diff, float dt);
        void project(float dt);
    };
}