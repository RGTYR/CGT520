// Planet.h
#ifndef __PLANET_H__
#define __PLANET_H__


#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Sphere.h"


class Planet: public Sphere
{
public:

	float planetRadius;
	float planetPos;
	float rotationSpeed;
	float revolutionSpeed;
	glm::vec3 rotate_axis;

	Planet(float radius, float pos, float rotation, float revolution, glm::vec3 rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f));
	~Planet();

	glm::mat4 getMatrix(float time_sec);

private:

};

#endif // !__PLANET_H__

