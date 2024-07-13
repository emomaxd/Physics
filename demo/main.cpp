#include "Pendulum.h"
#include "Cloth.h"
#include <iostream>



int main() {
    QP::Pendulum pendulum(QP::Vec3(0, 3.14 / 4, 0), 1.0, 9.81);
    QP::Cloth cloth(10, 10, 10, 10);

    double dt = 0.01;

    for (int i = 0; i < 1000; ++i) {
        pendulum.update(dt);
        std::cout << "Pendulum - Time: " << i * dt << " s, Position: " << pendulum.getPosition().y << " rad\n";

        cloth.update(dt);
        cloth.draw();
    }

    return 0;
}
