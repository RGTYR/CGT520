#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <glm/glm.hpp> //This header defines basic glm types (vec3, mat4, etc)
#include <GL/glew.h>
#include <vector>
#include <random>
#include <GL/freeglut.h>

struct Particle
{
	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec4 Colour;
	GLfloat Life;
	GLfloat Time;

	Particle(): Position(0.0f), Velocity(0.0f), Colour(1.0f), Life(1.0f), Time(0.0f) {}
};

class ParticleSystem
{
public:
	std::vector<Particle> particles;

	void add_particles(int num, float time_sec);
	Particle new_particle(float time_sec);

	ParticleSystem(glm::vec3 centre, int max_particles, int init_particles);
	~ParticleSystem();

private:

	glm::vec3 centre;
	int max_particles;

};

#endif // !__PARTICLE_H__
