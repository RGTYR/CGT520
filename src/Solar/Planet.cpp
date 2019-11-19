#include "Planet.h"

Planet::Planet(float radius, float pos, float rotation, float revolution)
{
	planetRadius = radius;
	planetPos = pos;
	rotationSpeed = rotation;
	revolutionSpeed = revolution;

}

Planet::~Planet()
{

}

glm::mat4 Planet::getMatrix(float time_sec)
{
	glm::mat4 M;

	glm::mat4 Rotation = glm::rotate(time_sec / rotationSpeed, glm::vec3(1.0f, 1.0f, 0.0f));

	glm::mat4 Scale = glm::scale(glm::vec3(planetRadius));

	glm::mat4 Trans = glm::translate(glm::vec3(planetPos, 0, 0));

	glm::mat4 Revolute = glm::rotate(time_sec / revolutionSpeed, glm::vec3(0.0f, 1.0f, 0.0f));

	M = Revolute * Trans * Scale * Rotation;

	return M;
}
