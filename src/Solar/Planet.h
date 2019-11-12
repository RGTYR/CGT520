// Planet.h
#ifndef __PLANET_H__
#define __PLANET_H__


#include <glm/glm.hpp>
#include <GL/glew.h>

class Planet
{
public:

	float planetRadius;
	glm::vec3 planetPos;
	float rotationSpeed;
	float revolutionSpeed;

	Planet(float radius, glm::vec3 pos, float rotation, float revolution);
	~Planet();

private:

};

#endif // !__PLANET_H__

