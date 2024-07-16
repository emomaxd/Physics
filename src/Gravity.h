#pragma once

#include "Vector.h"
#include "Timestep.h"

#include <vector>

namespace QP {

	struct Particle{
		Vec3 position;
		Vec3 acceleration;
		Vec3 velocity;
		float mass;
	};

	void ApplyForce(Particle& particle, const Vec3& force);

	void UpdateParticle(Particle& particle, Timestep ts);

	struct Gravity{
		std::vector<Particle*> particles;
	};

	void UpdateGravity(const Gravity& sim, Timestep ts);

}

