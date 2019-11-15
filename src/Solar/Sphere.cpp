#include "Sphere.h"

Sphere::Sphere(int stacks, int sectors, float radius)
{
	this->stacks = stacks;
	this->sectors = sectors;
	this->radius = radius;

	getVertices();
}

Sphere::~Sphere()
{

}

std::vector<glm::vec3> Sphere::getVertices()
{
	if (this->vertices.size() == 0)
	{
		std::vector<glm::vec3> points;

		for (int i = 0; i <= this->stacks; i++)
		{
			// the phi angle of these points
			float phi = glm::half_pi<float>() - glm::pi<float>() * (float)i / this->stacks;

			for (int j = 0; j <= this->sectors; j++)
			{
				// the theta angle of the point
				float theta = 2 * glm::pi<float>() * (float)j / this->sectors;

				float point_x = this->radius * cosf(phi) * cosf(theta);
				float point_y = this->radius * cosf(phi) * sinf(theta);
				float point_z = this->radius * sinf(phi);

				// use this line to debug
				// std::cout << point_x << ", " << point_y << ", " << point_z << std::endl;

				points.push_back(glm::vec3(point_x, point_y, point_z));
			}
		}

		for (int i = 0; i + this->stacks + 1 < points.size(); i++)
		{
			/*if (i % sectors == 0 && i != 0)
			{
				vertices.push_back(points[i]);
				vertices.push_back(points[i + this->stacks]);
				vertices.push_back(points[i + 1]);

				vertices.push_back(points[i + 1 - this->stacks]);
				vertices.push_back(points[i + this->stacks]);
				vertices.push_back(points[i + 1]);
			}
			else
			{
				
			}*/

			vertices.push_back(points[i]);
			vertices.push_back(points[i + this->stacks]);
			vertices.push_back(points[i + 1]);

			vertices.push_back(points[i + 1]);
			vertices.push_back(points[i + this->stacks]);
			vertices.push_back(points[i + this->stacks + 1]);
		}
	}

	return this->vertices;
}

GLuint Sphere::getVAO()
{
	GLuint vao;

	//Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
	glGenVertexArrays(1, &vao);

	//Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
	glBindVertexArray(vao);

	GLuint vbo_temp = this->getVBO();

	//See also InitShader.cpp line 164.
	const GLint pos_loc = 0;

	//Enable the position attribute.
	glEnableVertexAttribArray(pos_loc);

	//Tell opengl how to get the attribute values out of the vbo (stride and offset).
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);
	glBindVertexArray(0); //unbind the vao

	return vao;
}

GLuint Sphere::getVBO()
{
	GLuint vbo;

	//Generate vbo to hold vertex attributes for triangle.
	glGenBuffers(1, &vbo);

	//Specify the buffer where vertex attribute data is stored.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Upload from main memory to gpu memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);

	return vbo;
}

void Sphere::drawSphere(GLuint vao)
{
	// std::cout << vertices.size();
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
