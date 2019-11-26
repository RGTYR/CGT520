#include <windows.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <glm/glm.hpp> //This header defines basic glm types (vec3, mat4, etc)

//These headers define matrix transformations
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glm/gtc/type_ptr.hpp> //This header defines helper functions (e.g. value_ptr(...)) for passing vectors and matrices to shaders

#include "InitShader.h"	 //Functions for loading shaders from text files
#include "LoadMesh.h"		//Functions for creating OpenGL buffers from mesh files
#include "LoadTexture.h"	//Functions for creating OpenGL textures from image files
#include "imgui_impl_glut.h" //Functions for using imgui UI in glut programs

// Functions for camera
#include "Camera.h"
// Class of Sphere and Planet
#include "Sphere.h"
#include "Planet.h"
// Functions for skybox
#include "Cube.h"


static const std::string vertex_shader("gui_demo_vs.glsl");
static const std::string fragment_shader("gui_demo_fs.glsl");
//Texture map for fish
GLuint shader_program = -1;
GLuint texture_id = -1;

static const std::string mesh_name = "Amago0.obj";
static const std::string texture_name = "AmagoT.bmp";
MeshData mesh_data;

// Sphere Mesh
static const std::string sphere_mesh_name = "sphere.obj";
MeshData sphere_mesh_data;

// Cube files and IDs
static const std::string cube_vs("cube_vs.glsl");
static const std::string cube_fs("cube_fs.glsl");
GLuint cube_shader_program = -1;
GLuint cube_vao = -1;
// Texture id for cubemap
static const std::string cube_name = "cubemap";
GLuint cubemap_id = -1;


// Sun files and IDs
Planet sun(10.4, 0, 0.25, 100);
static const std::string sun_vs("sun_vs.glsl");
static const std::string sun_fs("sun_fs.glsl");
GLuint sun_shader_program = -1;
GLuint sun_texture_id = -1;
static const std::string sun_texture_name = "sunmap.bmp";

// Mercury files and IDs
Planet mercury(0.61, 67.0, 0.58, 0.87);
static const std::string mercury_vs("planet_vs.glsl");
static const std::string mercury_fs("planet_fs.glsl");
GLuint mercury_shader_program = -1;
GLuint mercury_texture_id = -1;
static const std::string mercury_texture_name = "mercurymap.bmp";

// venus files and IDs
Planet venus(0.97, 91.0, 2.43, 2.24);
static const std::string venus_vs("planet_vs.glsl");
static const std::string venus_fs("planet_fs.glsl");
GLuint venus_shader_program = -1;
GLuint venus_texture_id = -1;
static const std::string venus_texture_name = "venusmap.bmp";

// earth files and IDs
Planet earth(1, 109.0, 0.01, 3.65);
static const std::string earth_vs("planet_vs.glsl");
static const std::string earth_fs("planet_fs.glsl");
GLuint earth_shader_program = -1;
GLuint earth_texture_id = -1;
static const std::string earth_texture_name = "earthmap.bmp";

// mars files and IDs
Planet mars(0.73, 133.0, 0.01, 6.86);
static const std::string mars_vs("planet_vs.glsl");
static const std::string mars_fs("planet_fs.glsl");
GLuint mars_shader_program = -1;
GLuint mars_texture_id = -1;
static const std::string mars_texture_name = "marsmap.bmp";

// jupiter files and IDs
Planet jupiter(3.3, 246.0, 0.003, 40.0);
static const std::string jupiter_vs("planet_vs.glsl");
static const std::string jupiter_fs("planet_fs.glsl");
GLuint jupiter_shader_program = -1;
GLuint jupiter_texture_id = -1;
static const std::string jupiter_texture_name = "jupitermap.bmp";

// saturn files and IDs
Planet saturn(3.0, 334.0, 0.004, 10.0);
static const std::string saturn_vs("planet_vs.glsl");
static const std::string saturn_fs("planet_fs.glsl");
GLuint saturn_shader_program = -1;
GLuint saturn_texture_id = -1;
static const std::string saturn_texture_name = "saturnmap.bmp";

// uranus files and IDs
Planet uranus(2.0, 474.0, 0.006, 306.0);
static const std::string uranus_vs("planet_vs.glsl");
static const std::string uranus_fs("planet_fs.glsl");
GLuint uranus_shader_program = -1;
GLuint uranus_texture_id = -1;
static const std::string uranus_texture_name = "uranusmap.bmp";

// neptune files and IDs
Planet neptune(2.0, 594.0, 0.006, 606.0);
static const std::string neptune_vs("planet_vs.glsl");
static const std::string neptune_fs("planet_fs.glsl");
GLuint neptune_shader_program = -1;
GLuint neptune_texture_id = -1;
static const std::string neptune_texture_name = "neptunemap.bmp";

// asteroid files and IDs
static const std::string asteroid_vs("planet_vs.glsl");
static const std::string asteroid_fs("planet_fs.glsl");
GLuint asteroid_shader_program = -1;
MeshData deimos_mesh_data;
static const std::string deimos_mesh_name = "deimos.obj";
static const std::string deimos_texture_name = "deimosbump.bmp";
GLuint deimos_texture_id = -1;
MeshData phobos_mesh_data;
static const std::string phobos_mesh_name = "phobos.obj";
static const std::string phobos_texture_name = "phobosbump.bmp";
GLuint phobos_texture_id = -1;

float angle = 0.0f;
float scale = 1.0f;
Camera camera(glm::vec3(-10.0f, 10.0f, 30.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//Draw the ImGui user interface
void draw_gui()
{
	ImGui_ImplGlut_NewFrame();

	//ImGui::Begin("Debug menu");
	////uncomment the following line to create a sliders which changes the viewing angle and scale
	//ImGui::SliderFloat("View angle", &angle, -180.0f, +180.0f);
	//ImGui::SliderFloat("Scale", &scale, -10.0f, +10.0f);
	//ImGui::SliderFloat3("Camera Position", &camera.cameraPos.x, -10.0f, +10.0f);
	//ImGui::End();

	static bool show_test = false;
	ImGui::ShowTestWindow(&show_test);

	ImGui::Render();
}


void draw_cube(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(cube_shader_program);
	int PVM_loc = glGetUniformLocation(cube_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 Msky = glm::scale(glm::vec3(10000.0f));
		glm::mat4 PVM = P * V * Msky;
		PVM[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id);
	int cube_loc = glGetUniformLocation(cube_shader_program, "cubemap");
	if (cube_loc != -1)
	{
		glUniform1i(cube_loc, 1); // we bound our texture to texture unit 1
	}

	glDepthMask(GL_FALSE);
	glBindVertexArray(cube_vao);
	draw_cube(cube_vao);
	glDepthMask(GL_TRUE);
}


void draw_fish(const glm::mat4& V, const glm::mat4& P)
{
	glm::mat4 M = glm::rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f))*glm::scale(glm::vec3(scale*mesh_data.mScaleFactor));

	glUseProgram(shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	int PVM_loc = glGetUniformLocation(shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int P_loc = glGetUniformLocation(shader_program, "P");
	if (P_loc != -1)
	{
		glUniformMatrix4fv(P_loc, 1, false, glm::value_ptr(P));
	}

	int VM_loc = glGetUniformLocation(shader_program, "VM");
	if (VM_loc != -1)
	{
		glm::mat4 VM = V * M;
		glUniformMatrix4fv(VM_loc, 1, false, glm::value_ptr(VM));
	}


	int tex_loc = glGetUniformLocation(shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	glBindVertexArray(mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	mesh_data.DrawMesh();

}

void draw_sun(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(sun_shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sun_texture_id);

	int PVM_loc = glGetUniformLocation(sun_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		const int time_ms = glutGet(GLUT_ELAPSED_TIME);
		float time_sec = 0.001f*time_ms;

		// glm::mat4 R = glm::rotate(sin(time_sec), glm::vec3(1.0f, 1.0f, 0.0f));
		// glm::mat4 M = R * glm::scale(glm::vec3(1.0f));
		glm::mat4 M = sun.getMatrix(time_sec) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(sun_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();

	/*glBindVertexArray(sun_vao);
	sun.drawSphere(sun_vao);*/
}

void draw_mercury(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(mercury_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mercury_texture_id);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	glm::mat4 M = mercury.getMatrix(time_sec) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(mercury_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(mercury_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(mercury_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(mercury_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();

	/*glBindVertexArray(mercury_vao);
	sun.drawSphere(mercury_vao);*/
}

void draw_venus(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(venus_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, venus_texture_id);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	glm::mat4 M = venus.getMatrix(time_sec) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(venus_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(venus_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(venus_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(venus_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();

	/*glBindVertexArray(venus_vao);
	sun.drawSphere(venus_vao);*/
}

void draw_earth(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(earth_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, earth_texture_id);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	glm::mat4 M = earth.getMatrix(time_sec) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(earth_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(earth_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(earth_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(earth_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();

	/*glBindVertexArray(earth_vao);
	sun.drawSphere(earth_vao);*/
}

void draw_mars(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(mars_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mars_texture_id);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	glm::mat4 M = mars.getMatrix(time_sec) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(mars_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(mars_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(mars_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(mars_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();

}

void draw_jupiter(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(jupiter_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, jupiter_texture_id);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	glm::mat4 M = jupiter.getMatrix(time_sec) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(jupiter_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(jupiter_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(jupiter_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(jupiter_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();

}

void draw_saturn(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(saturn_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, saturn_texture_id);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	glm::mat4 M = saturn.getMatrix(time_sec) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(saturn_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(saturn_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(saturn_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(saturn_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();

}

void draw_uranus(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(uranus_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, uranus_texture_id);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	glm::mat4 M = uranus.getMatrix(time_sec) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(uranus_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(uranus_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(uranus_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(uranus_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();

}

void draw_neptune(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(neptune_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, neptune_texture_id);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	glm::mat4 M = neptune.getMatrix(time_sec) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(neptune_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(neptune_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(neptune_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(neptune_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();

}

void draw_deimos(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(asteroid_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deimos_texture_id);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;
	glm::mat4 Scale = glm::scale(glm::vec3(0.01f));
	// To do
	glm::mat4 Trans = glm::translate(glm::vec3(0 , 0, 0));
	glm::mat4 M = glm::scale(glm::vec3(scale * deimos_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(neptune_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(neptune_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(neptune_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(neptune_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(deimos_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	deimos_mesh_data.DrawMesh();

}



// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer

	glm::mat4 V = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraTarget, camera.worldUp);
	glm::mat4 P = glm::perspective(40.0f, 1.0f, 0.1f, 10000.0f);

	// draw_fish(V, P);
	draw_sun(V, P);
	draw_mercury(V, P);
	draw_venus(V, P);
	draw_earth(V, P);
	draw_mars(V, P);
	draw_jupiter(V, P);
	draw_saturn(V, P);
	draw_uranus(V, P);
	draw_neptune(V, P);
	
	draw_cube(V, P);

	draw_gui();

	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;

	//Pass time_sec value to the shaders
	int time_loc = glGetUniformLocation(shader_program, "time");
	if (time_loc != -1)
	{
		glUniform1f(time_loc, time_sec);
	}
}

void reload_shader()
{
	GLuint new_shader = InitShader(vertex_shader.c_str(), fragment_shader.c_str());

	if (new_shader == -1) // loading failed
	{
		glClearColor(1.0f, 0.0f, 1.0f, 0.0f); //change clear color if shader can't be compiled
	}
	else
	{
		glClearColor(0.35f, 0.35f, 0.35f, 0.0f);

		if (shader_program != -1)
		{
			glDeleteProgram(shader_program);
		}
		shader_program = new_shader;
	}
}

void printGlInfo()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void initOpenGl()
{
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SPRITE);       // allows textured points
	glEnable(GL_PROGRAM_POINT_SIZE); //allows us to set point size in vertex shader

	reload_shader();

	//mesh and texture to be rendered
	mesh_data = LoadMesh(mesh_name);
	texture_id = LoadTexture(texture_name);
	
	// sphere mesh
	sphere_mesh_data = LoadMesh(sphere_mesh_name);

	// create Sun
	sun_shader_program = InitShader(sun_vs.c_str(), sun_fs.c_str());
	sun_texture_id = LoadTexture(sun_texture_name);

	// create Mercury
	mercury_shader_program = InitShader(mercury_vs.c_str(), mercury_fs.c_str());
	mercury_texture_id = LoadTexture(mercury_texture_name);

	// create Venus
	venus_shader_program = InitShader(venus_vs.c_str(), venus_fs.c_str());
	venus_texture_id = LoadTexture(venus_texture_name);

	// create Earth
	earth_shader_program = InitShader(earth_vs.c_str(), earth_fs.c_str());
	earth_texture_id = LoadTexture(earth_texture_name);

	// create Mars
	mars_shader_program = InitShader(mars_vs.c_str(), mars_fs.c_str());
	mars_texture_id = LoadTexture(mars_texture_name);

	// create Jupiter
	jupiter_shader_program = InitShader(jupiter_vs.c_str(), jupiter_fs.c_str());
	jupiter_texture_id = LoadTexture(jupiter_texture_name);

	// create Saturn
	saturn_shader_program = InitShader(saturn_vs.c_str(), saturn_fs.c_str());
	saturn_texture_id = LoadTexture(saturn_texture_name);

	// create Uranus
	uranus_shader_program = InitShader(uranus_vs.c_str(), uranus_fs.c_str());
	uranus_texture_id = LoadTexture(uranus_texture_name);

	// create Neptune
	neptune_shader_program = InitShader(neptune_vs.c_str(), neptune_fs.c_str());
	neptune_texture_id = LoadTexture(neptune_texture_name);

	// create Asteroid
	deimos_mesh_data = LoadMesh(deimos_mesh_name);
	deimos_texture_id = LoadTexture(deimos_texture_name);
	phobos_mesh_data = LoadMesh(phobos_mesh_name);
	phobos_texture_id = LoadTexture(phobos_texture_name);

	// skybox
	cubemap_id = LoadCube(cube_name);
	cube_shader_program = InitShader(cube_vs.c_str(), cube_fs.c_str());
	cube_vao = create_cube_vao();

	// initialize the imgui system
	ImGui_ImplGlut_Init();
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
	ImGui_ImplGlut_KeyCallback(key);

	switch (key)
	{
	case 'r':
	case 'R':
		reload_shader();
		break;

	case 'w':
	case 'W':
		camera.move(FORWARD);
		break;

	case 's':
	case 'S':
		camera.move(BACKWARD);
		break;

	case 'a':
	case 'A':
		camera.move(LEFT);
		break;

	case 'd':
	case 'D':
		camera.move(RIGHT);
		break;

	case 'q':
	case 'Q':
		camera.move(UP);
		break;

	case 'e':
	case 'E':
		camera.move(DOWN);
		break;
	}
}

void keyboard_up(unsigned char key, int x, int y)
{
	ImGui_ImplGlut_KeyUpCallback(key);
}

void special_up(int key, int x, int y)
{
	ImGui_ImplGlut_SpecialUpCallback(key);
}

void passive(int x, int y)
{
	ImGui_ImplGlut_PassiveMouseMotionCallback(x, y);
}

void special(int key, int x, int y)
{
	ImGui_ImplGlut_SpecialCallback(key);
}

void motion(int x, int y)
{
	ImGui_ImplGlut_MouseMotionCallback(x, y);

	camera.cameraTarget = camera.cameraDir(x, y, 0.2f);
}

void mouse(int button, int state, int x, int y)
{
	ImGui_ImplGlut_MouseButtonCallback(button, state);
}


int main(int argc, char **argv)
{
	//Configure initial window state
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(5, 5);
	glutInitWindowSize(1080, 1080);
	int win = glutCreateWindow("GUI demo");

	printGlInfo();

	//Register callback functions with glut. 
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialUpFunc(special_up);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);
	glutIdleFunc(idle);

	initOpenGl();
	ImGui_ImplGlut_Init(); // initialize the imgui system

	//Enter the glut event loop.
	glutMainLoop();
	glutDestroyWindow(win);
	return 0;
}