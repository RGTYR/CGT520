#include "Planet.h"

Planet::Planet(float radius, glm::vec3 pos, float rotation, float revolution)
{
	planetRadius = radius;
	planetPos = pos;
	rotationSpeed = rotation;
	revolutionSpeed = revolution;
}

Planet::~Planet()
{

}
