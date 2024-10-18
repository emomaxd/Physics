#pragma once

#include "Vector.h"
#include "Timestep.h"

#include <vector>

namespace QP {

	struct GravityParticle{
		Vec3 position;
		Vec3 acceleration;
		Vec3 velocity;
		float mass;
	};

	void ApplyForce(GravityParticle& particle, const Vec3& force);

	void UpdateParticle(GravityParticle& particle, float ts);

	struct Gravity{
		std::vector<GravityParticle> particles;
	};

	void InitializeParticles(Gravity& sim, int count, float range);

	void UpdateGravity(Gravity& sim, float ts);

}

