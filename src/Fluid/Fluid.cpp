#include "Fluid.h"
#include <cmath>
#include <algorithm>

namespace QP {

    FluidSimulation::FluidSimulation(int width, int height)
        : width(width), height(height), grid(width, std::vector<FluidCell>(height)) {
        initializeGrid();
    }

    void FluidSimulation::initializeGrid() {
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                grid[x][y] = { 0.0f, 0.0f, {0.0f, 0.0f}, false };
            }
        }

        for (int x = 20; x < 30; ++x) {
            for (int y = 20; y < 30; ++y) {
                grid[x][y].obstacle = true;
            }
        }
    }

    void FluidSimulation::advect(float dt) {
        std::vector<std::vector<FluidCell>> tempGrid = grid;

        for (int x = 1; x < width - 1; ++x) {
            for (int y = 1; y < height - 1; ++y) {
                if (grid[x][y].obstacle) continue;

                float x0 = x - grid[x][y].velocity.u * dt;
                float y0 = y - grid[x][y].velocity.v * dt;

                x0 = std::max(0.0f, std::min(x0, static_cast<float>(width - 1)));
                y0 = std::max(0.0f, std::min(y0, static_cast<float>(height - 1)));

                int x0_floor = static_cast<int>(std::floor(x0));
                int y0_floor = static_cast<int>(std::floor(y0));

                float t_x = x0 - x0_floor;
                float t_y = y0 - y0_floor;

                if (x0_floor >= 0 && x0_floor + 1 < width && y0_floor >= 0 && y0_floor + 1 < height) {
                    tempGrid[x][y].smoke = (1 - t_x) * (1 - t_y) * grid[x0_floor][y0_floor].smoke +
                        t_x * (1 - t_y) * grid[x0_floor + 1][y0_floor].smoke +
                        (1 - t_x) * t_y * grid[x0_floor][y0_floor + 1].smoke +
                        t_x * t_y * grid[x0_floor + 1][y0_floor + 1].smoke;

                    tempGrid[x][y].velocity.u = (1 - t_x) * (1 - t_y) * grid[x0_floor][y0_floor].velocity.u +
                        t_x * (1 - t_y) * grid[x0_floor + 1][y0_floor].velocity.u +
                        (1 - t_x) * t_y * grid[x0_floor][y0_floor + 1].velocity.u +
                        t_x * t_y * grid[x0_floor + 1][y0_floor + 1].velocity.u;

                    tempGrid[x][y].velocity.v = (1 - t_x) * (1 - t_y) * grid[x0_floor][y0_floor].velocity.v +
                        t_x * (1 - t_y) * grid[x0_floor + 1][y0_floor].velocity.v +
                        (1 - t_x) * t_y * grid[x0_floor][y0_floor + 1].velocity.v +
                        t_x * t_y * grid[x0_floor + 1][y0_floor + 1].velocity.v;
                }
            }
        }

        grid = tempGrid;
    }

    void FluidSimulation::diffuse(float diff, float dt) {
        std::vector<std::vector<FluidCell>> tempGrid = grid;

        for (int k = 0; k < 20; ++k) { // 20 iterations for Gauss-Seidel relaxation
            for (int x = 1; x < width - 1; ++x) {
                for (int y = 1; y < height - 1; ++y) {
                    if (grid[x][y].obstacle) continue;

                    tempGrid[x][y].smoke = (grid[x][y].smoke + diff * dt * (
                        grid[x + 1][y].smoke +
                        grid[x - 1][y].smoke +
                        grid[x][y + 1].smoke +
                        grid[x][y - 1].smoke)) / (1 + 4 * diff * dt);
                }
            }
        }

        grid = tempGrid;
    }

    void FluidSimulation::project(float dt) {
        std::vector<std::vector<float>> div(width, std::vector<float>(height, 0.0f));
        std::vector<std::vector<float>> p(width, std::vector<float>(height, 0.0f));

        for (int x = 1; x < width - 1; ++x) {
            for (int y = 1; y < height - 1; ++y) {
                if (grid[x][y].obstacle) continue;

                div[x][y] = -0.5f * (grid[x + 1][y].velocity.u - grid[x - 1][y].velocity.u +
                    grid[x][y + 1].velocity.v - grid[x][y - 1].velocity.v) / width;
                p[x][y] = 0.0f;
            }
        }

        for (int k = 0; k < 20; ++k) { // 20 iterations for Gauss-Seidel relaxation
            for (int x = 1; x < width - 1; ++x) {
                for (int y = 1; y < height - 1; ++y) {
                    if (grid[x][y].obstacle) continue;

                    p[x][y] = (div[x][y] + p[x + 1][y] + p[x - 1][y] + p[x][y + 1] + p[x][y - 1]) / 4.0f;
                }
            }
        }

        for (int x = 1; x < width - 1; ++x) {
            for (int y = 1; y < height - 1; ++y) {
                if (grid[x][y].obstacle) continue;

                grid[x][y].velocity.u -= 0.5f * width * (p[x + 1][y] - p[x - 1][y]);
                grid[x][y].velocity.v -= 0.5f * height * (p[x][y + 1] - p[x][y - 1]);
            }
        }
    }

    void FluidSimulation::update(float dt) {
        advect(dt);
        diffuse(0.1f, dt); // diffusion coefficient
        project(dt);
    }

    void FluidSimulation::addSmoke(int x, int y, float amount) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[x][y].smoke += amount;
        }
    }

    void FluidSimulation::addVelocity(int x, int y, float u, float v) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[x][y].velocity.u += u;
            grid[x][y].velocity.v += v;
        }
    }

    const std::vector<std::vector<FluidCell>>& FluidSimulation::getGrid() const {
        return grid;
    }
}