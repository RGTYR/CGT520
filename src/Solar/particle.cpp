#include "particle.h"

#include <random>

std::default_random_engine e;

double randomFloat(double a, double b)
{
	std::uniform_real_distribution<double> u(a, b);
	return u(e);
}


void ParticleSystem::add_particles(int num, float time_sec)
{
	for (int i = 0; i < num && particles.size() < max_particles; i++)
	{
		Particle particle_temp = new_particle(time_sec);
		particles.push_back(particle_temp);
	}
}

Particle ParticleSystem::new_particle(float time_sec)
{

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 velocity = glm::vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f));
	glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float life = 1.0f;
	Particle particle_temp;
	particle_temp.Colour = colour;
	particle_temp.Position = position;
	particle_temp.Life = life;
	particle_temp.Velocity = velocity;
	particle_temp.Time = time_sec;

	return particle_temp;
}

ParticleSystem::ParticleSystem(glm::vec3 centre, int max_particles, int init_particles)
{
	for (int i = 0; i < init_particles; i++)
	{
		Particle particle_temp = new_particle(0.0f);
		particles.push_back(particle_temp);
	}

	this->max_particles = max_particles;
}

ParticleSystem::~ParticleSystem()
{

}
