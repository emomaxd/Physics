#include "Gravity.h"


namespace QP {

	static void GravityForce(Particle& p1, Particle& p2){
		
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

	void ApplyForce(Particle& particle, const Vec3& force){
		particle.acceleration += force / particle.mass;
	}

	void UpdateParticle(Particle& particle, Timestep ts){
		particle.velocity += particle.acceleration * ts;
		particle.position += particle.velocity * ts;
	}
	
	void UpdateGravity(Gravity& gravity, Timestep ts){

		auto& particles = gravity.particles;

		for(auto& particle : particles){
			particle->acceleration = Vec3(0, 0, 0);
		}

		for(size_t i = 0; i < particles.size(); ++i){
			for(size_t j = i + 1; j < particles.size(); ++j){
				GravityForce(*particles[i], *particles[j]);
			}
		}

		for(auto& particle : particles){
			UpdateParticle(*particle, ts);
		}   	
	}


}
