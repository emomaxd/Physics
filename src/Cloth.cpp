#include "Cloth.h"

namespace QP {


	Particle::Particle(float x, float y) {
		position = { x, y };
		oldPosition = { x, y };
		acceleration = { 0, 0 };
		isStatic = false;
	}

	void Particle::applyForce(const Vec2& force) {
		if (isStatic) return;
		acceleration += force;
	}

	void Particle::update(float deltaTime) {
		if (isStatic) return;

		Vec2 temp = position;
		Vec2 velocity = position - oldPosition;

		// Verlet integration
		position += velocity + acceleration * deltaTime * deltaTime;
		oldPosition = temp;

		acceleration = { 0, 0 }; // Reset acceleration
	}








  
	Cloth::Cloth(size_t width, size_t height) : width(width), height(height)
	{
		/// Create particles
		for(size_t y = 0; y < height; ++y)
			for(size_t x = 0; x < width; ++x)
				particles.emplace_back(x * 0.1f, y * 0.1f);
		
		for(size_t y = 0; y < height; ++y)
			for(size_t x = 0; x < width; ++x){
				if(x < width - 1)
					constraints.emplace_back(y * width + x, y * width + (x + 1));
				if(y < height - 1)
					constraints.emplace_back(y * width + x, (y + 1) * width + x);
			}

		particles[0].isStatic = true;
		particles[width - 1].isStatic = true;
	}

	void Cloth::applyGravity()
	{
		Vec2 gravity = {0.0f, -9.81f};
		for (auto& particle : particles)
		{
			if (particle.isStatic) continue;
			particle.applyForce(gravity);

		}
			
	}

	void Cloth::applyForce(const Vec2& force)
	{
		for (auto& particle : particles)
			particle.applyForce(force);
	}

	void Cloth::applyMouseForce(const Vec2& force)
	{
		for (auto& particle : particles)
		{
			Vec2 dir = force - particle.position;
			particle.applyForce(dir);
		}
			
	}

	void Cloth::update(float ts)
	{
		applyGravity();
		for (auto& particle : particles)
			particle.update(ts);
			

		// Resolve constraints
		for(int i = 0; i < 25; ++i)
			for(const auto& constraint : constraints)
				resolveConstraint(constraint.first, constraint.second);
	}

	void Cloth::resolveConstraint(int a, int b)
	{
		Vec2 delta = particles[b].position - particles[a].position;
		float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
		float restLength = 0.1f;

		if(distance != restLength)
		{
			float diff = (distance - restLength) / distance;
			Vec2 correction = delta * 0.5f * diff;

			if(!particles[a].isStatic)particles[a].position += correction;
			if(!particles[b].isStatic)particles[b].position -= correction;
		}
	}


} // namespace QP
