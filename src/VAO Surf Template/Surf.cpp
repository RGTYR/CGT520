#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> //for pi
#include "Surf.h"

static int N = 50;

//The surface to draw.
glm::vec3 surf(int i, int j)
{
   const float center = 0.5f*N;
   const float xy_scale = 20.0f / N;
   const float z_scale = 10.0f;

   float x = xy_scale * (i - center);
   float y = xy_scale * (j - center);

   float r = sqrt(x*x + y * y);
   float z = 1.0f;

   if (r != 0.0f)
   {
      z = sin(r) / r;
   }
   z = z * z_scale;

   return 0.05f*glm::vec3(x, y, z);
}

//The demo shape being currently draw.
glm::vec3 circle(int i)
{
   const float r = 1.0f;
   float theta = i * 2.0f*glm::pi<float>() / N;
   return glm::vec3(r*cos(theta), r*sin(theta), 0.0f);
}

GLuint create_surf_vbo()
{
   //Declare a vector to hold N vertices
   std::vector<glm::vec3> surf_verts(N * N);

   for (int i = 0; i < N; i++)
   {
	   for (int j = 0; j < N; j++)
	   {
		   surf_verts[i * N + j] = surf(i, j);
	   }
   }

   GLuint vbo;
   glGenBuffers(1, &vbo); //Generate vbo to hold vertex attributes for triangle.

   glBindBuffer(GL_ARRAY_BUFFER, vbo); //Specify the buffer where vertex attribute data is stored.
   
   //Upload from main memory to gpu memory.
   glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*surf_verts.size(), surf_verts.data(), GL_STATIC_DRAW);

   return vbo;
}

GLuint create_surf_vao()
{
   GLuint vao;

   //Generate vao id to hold the mapping from attrib variables in shader to memory locations in vbo
   glGenVertexArrays(1, &vao);

   //Binding vao means that bindbuffer, enablevertexattribarray and vertexattribpointer state will be remembered by vao
   glBindVertexArray(vao);

   GLuint vbo = create_surf_vbo();

   const GLint pos_loc = 0; //See also InitShader.cpp line 164.

   glEnableVertexAttribArray(pos_loc); //Enable the position attribute.

   //Tell opengl how to get the attribute values out of the vbo (stride and offset).
   glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);
   glBindVertexArray(0); //unbind the vao

   return vao;

}

GLuint create_triangles_vbo()
{
	//Declare a vector to hold N vertices
	std::vector<glm::vec3> surf_verts((N - 1) * (N - 1) * 2 * 3);

	for (int i = 0; i < N - 1; i++)
	{
		for (int j = 0; j < N - 1; j++)
		{
			surf_verts[(i * (N - 1) + j) * 6] = surf(i, j);
			surf_verts[(i * (N - 1) + j) * 6 + 1] = surf(i + 1, j);
			surf_verts[(i * (N - 1) + j) * 6 + 2] = surf(i, j + 1);
			surf_verts[(i * (N - 1) + j) * 6 + 3] = surf(i, j + 1);
			surf_verts[(i * (N - 1) + j) * 6 + 4] = surf(i + 1, j);
			surf_verts[(i * (N - 1) + j) * 6 + 5] = surf(i + 1, j + 1);
		}
	}

	GLuint vbo;
	glGenBuffers(1, &vbo); //Generate vbo to hold vertex attributes for triangle.

	glBindBuffer(GL_ARRAY_BUFFER, vbo); //Specify the buffer where vertex attribute data is stored.

	//Upload from main memory to gpu memory.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*surf_verts.size(), surf_verts.data(), GL_STATIC_DRAW);

	return vbo;
}

GLuint create_triangles_vao()
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo = create_triangles_vbo();

	const GLint pos_loc = 0;
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, false, 0, 0);
	glBindVertexArray(0);

	return vao;
	
}

void draw_surf_points(GLuint vao)
{
   glDrawArrays(GL_POINTS, 0, (N - 1) * (N - 1) * 2 * 3);
}

void draw_surf_triangles(GLuint vao)
{
	glDrawArrays(GL_TRIANGLES, 0, (N - 1) * (N - 1) * 2 * 3);
}
