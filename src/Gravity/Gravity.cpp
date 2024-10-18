#include "Gravity.h"


namespace QP {

	static void GravityForce(GravityParticle& p1, GravityParticle& p2){
		
		const float m1 = p1.mass;
		const float m2 = p2.mass;

		const float G = 6.674 * std::pow(10, -11);
		const Vec3 diffVector = (p1.position - p2.position); // A Vector From p2 to p1
		const Vec3 direction = diffVector.normalized();
		const float r = diffVector.length(); 
		const float F = G * m1 * m2 / (r * r);

		const Vec3 Force = direction * F;

		ApplyForce(p1, Force * -1);
		ApplyForce(p2, Force);
	}

	void ApplyForce(GravityParticle& particle, const Vec3& force){
		particle.acceleration += force / particle.mass;
	}

	void UpdateParticle(GravityParticle& particle, float ts){
		particle.velocity += particle.acceleration * ts;
		particle.position += particle.velocity * ts;
	}

	void InitializeParticles(Gravity& sim, int count, float range) {
		sim.particles.resize(count);
		//std::srand(static_cast<unsigned int>(std::time(nullptr)));

		for (int i = 0; i < count; ++i) {
			GravityParticle particle;
			particle.position = {
				static_cast<float>(std::rand()) / RAND_MAX * range - range / 2,
				static_cast<float>(std::rand()) / RAND_MAX * range - range / 2,
				static_cast<float>(std::rand()) / RAND_MAX * range - range / 2
			};

			// Random velocity between 0 and 20
			particle.velocity = {
				static_cast<float>(std::rand()) / RAND_MAX * 0.2f,
				static_cast<float>(std::rand()) / RAND_MAX * 0.2f,
				static_cast<float>(std::rand()) / RAND_MAX * 0.2f
			};

			particle.acceleration = { 0.0f, 0.0f, 0.0f };

			// Mass between 1 million and 10 billion
			particle.mass = static_cast<float>(std::rand()) / RAND_MAX * (10e9f - 1e6f) + 1e6f;

			sim.particles[i] = particle;
		}
	}
	
	void UpdateGravity(Gravity& gravity, float ts){

		auto& particles = gravity.particles;

		for(auto& particle : particles){
			particle.acceleration = Vec3(0, 0, 0);
		}

		for(size_t i = 0; i < particles.size(); ++i){
			for(size_t j = i + 1; j < particles.size(); ++j){
				GravityForce(particles[i], particles[j]);
			}
		}

		for(auto& particle : particles){
			UpdateParticle(particle, ts);
		}   	
	}


}
