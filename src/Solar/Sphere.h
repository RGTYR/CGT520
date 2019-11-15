#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <vector>

class Sphere
{
public:
	Sphere(int stacks, int sectors, float radius);
	~Sphere();

	// calculate and return vector which stores the vectices of sphere
	// algorithem from http://www.songho.ca/opengl/gl_sphere.html
	std::vector<glm::vec3> getVertices();
	// return VAO buffer id;
	GLuint getVAO();
	// return VBO buffer id;
	GLuint getVBO();

	void drawSphere(GLuint vao);

private:

	// latitude of sphere
	int stacks;
	// longitude of sphere
	int sectors;
	// radius of sphere
	float radius;

	// vector which stores the vectices of sphere
	std::vector<glm::vec3> vertices;
};

#endif // !__SPHERE_H_
