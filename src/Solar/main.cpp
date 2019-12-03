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

// Audio Player
#include <irrKlang/irrKlang.h>
#pragma comment(lib, "irrKlang.lib")

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

// Function for particles
#include "particle.h"

// global variables
#include "global_variables.h"


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

		// glm::mat4 R = glm::rotate(sin(time_sec), glm::vec3(1.0f, 1.0f, 0.0f));
		// glm::mat4 M = R * glm::scale(glm::vec3(1.0f));
		glm::mat4 M = sun.getMatrix(myTime.accountingTime) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));
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

	glm::mat4 M = mercury.getMatrix(myTime.accountingTime) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

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

	glm::mat4 M = venus.getMatrix(myTime.accountingTime) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

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

	glm::mat4 M = earth.getMatrix(myTime.accountingTime) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

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

void draw_moon(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(moon_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, moon_texture_id);

	glm::mat4 Trans = glm::translate(glm::vec3(1.5f * cos(myTime.accountingTime), 0.0f, 1.5f * sin(myTime.accountingTime)));
	glm::mat4 M = Trans * earth.getMatrix(myTime.accountingTime)
		* glm::scale(glm::vec3(0.3f * scale * sphere_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(moon_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(moon_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(moon_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(moon_shader_program, "V");
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

	glm::mat4 M = mars.getMatrix(myTime.accountingTime) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

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

	glm::mat4 M = jupiter.getMatrix(myTime.accountingTime) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

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

	glm::mat4 M = saturn.getMatrix(myTime.accountingTime) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

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

	glBindVertexArray(saturn_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	saturn_mesh_data.DrawMesh();

}

void draw_uranus(const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(uranus_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, uranus_texture_id);

	glm::mat4 M = uranus.getMatrix(myTime.accountingTime) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

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

	glm::mat4 M = neptune.getMatrix(myTime.accountingTime) * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

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

void draw_deimos(const glm::mat4& V, const glm::mat4& P, int index)
{
	glUseProgram(asteroid_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deimos_texture_id);

	glm::mat4 Scale = glm::scale(glm::vec3(0.2f));
	glm::mat4 Rotate = glm::rotate(deimos_rotation[index], glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Trans = glm::translate(glm::vec3(deimos_revolution_radius[index] * sin(deimos_revolution_theta[index]),
		0, deimos_revolution_radius[index] * cos(deimos_revolution_theta[index])));
	glm::mat4 Revolute = glm::rotate(myTime.accountingTime, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 M = Revolute * Trans * Rotate * Scale * glm::scale(glm::vec3(scale * deimos_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(asteroid_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(asteroid_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(asteroid_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(asteroid_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(deimos_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	deimos_mesh_data.DrawMesh();

}

void draw_phobos(const glm::mat4& V, const glm::mat4& P, int index)
{
	glUseProgram(asteroid_shader_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, phobos_texture_id);

	glm::mat4 Scale = glm::scale(glm::vec3(0.2f));
	glm::mat4 Rotate = glm::rotate(phobos_rotation[index], glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Trans = glm::translate(glm::vec3(phobos_revolution_radius[index] * sin(phobos_revolution_theta[index]),
		0, phobos_revolution_radius[index] * cos(phobos_revolution_theta[index])));
	glm::mat4 Revolute = glm::rotate(myTime.accountingTime, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 M = Revolute * Trans * Rotate * Scale * glm::scale(glm::vec3(scale * phobos_mesh_data.mScaleFactor));

	int PVM_loc = glGetUniformLocation(asteroid_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(asteroid_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 0); // we bound our texture to texture unit 1
	}

	int M_loc = glGetUniformLocation(asteroid_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(asteroid_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(phobos_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	phobos_mesh_data.DrawMesh();

}

void draw_particle(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P)
{
	glUseProgram(particles_shader_program);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sun_texture_id);

	int PVM_loc = glGetUniformLocation(particles_shader_program, "PVM");
	if (PVM_loc != -1)
	{
		glm::mat4 PVM = P * V * M;
		glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
	}

	int tex_loc = glGetUniformLocation(particles_shader_program, "diffuse_tex");
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, 1); // we bound our texture to texture unit 0
	}

	int M_loc = glGetUniformLocation(particles_shader_program, "M");
	if (M_loc != -1)
	{
		glUniformMatrix4fv(M_loc, 1, false, glm::value_ptr(M));
	}

	int V_loc = glGetUniformLocation(particles_shader_program, "V");
	if (V_loc != -1)
	{
		glUniformMatrix4fv(V_loc, 1, false, glm::value_ptr(V));
	}

	glBindVertexArray(sphere_mesh_data.mVao);
	// glDrawElements(GL_TRIANGLES, mesh_data.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
	// For meshes with multiple submeshes use mesh_data.DrawMesh();
	sphere_mesh_data.DrawMesh();
}

void draw_particles(const glm::mat4& V, const glm::mat4& P)
{

	for (int i = 0; i < p_sys.particles.size(); i++)
	{
		glm::mat4 Scale = glm::scale(glm::vec3(1.0f) * p_sys.particles[i].Life);
		glm::mat4 Trans = glm::translate(p_sys.particles[i].Velocity * (myTime.accountingTime - p_sys.particles[i].Time));
		glm::mat4 Revolute = glm::rotate((myTime.accountingTime - p_sys.particles[i].Time), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 M = Revolute * Trans * Scale * glm::scale(glm::vec3(scale * sphere_mesh_data.mScaleFactor));

		draw_particle(M, V, P);

		std::uniform_real_distribution<double> u(0.95, 1.0);
		p_sys.particles[i].Life = p_sys.particles[i].Life * u(random_engine);
	}

	for (int i = 0; i < p_sys.particles.size(); i++)
	{
		if (p_sys.particles[i].Life < 0.1)
		{
			p_sys.particles[i] = p_sys.new_particle(myTime.accountingTime);
		}
	}

	p_sys.add_particles(20, myTime.accountingTime);
}

// glut display callback function.
// This function gets called every time the scene gets redisplayed 
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the back buffer

	glm::mat4 V = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraTarget, camera.worldUp);
	glm::mat4 P = glm::perspective(40.0f, 1.0f, 0.1f, 10000.0f);

	const int time_ms = glutGet(GLUT_ELAPSED_TIME);
	float time_sec = 0.001f*time_ms;

	myTime.currentTime = time_sec;
	myTime.accountingTime += (myTime.currentTime - myTime.lastTime) * myTime.timeSpeed;
	myTime.lastTime = myTime.currentTime;

	// draw_fish(V, P);
	draw_sun(V, P);
	draw_mercury(V, P);
	draw_venus(V, P);
	draw_earth(V, P);
	draw_moon(V, P);
	draw_mars(V, P);
	draw_jupiter(V, P);
	draw_saturn(V, P);
	draw_uranus(V, P);
	draw_neptune(V, P);
	for (int i = 0; i < 500; i++)
		draw_deimos(V, P, i);
	for (int i = 0; i < 500; i++)
		draw_phobos(V, P, i);
	
	draw_cube(V, P);

	draw_particles(V, P);

	// draw_gui();

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
	// create Moon
	moon_shader_program = InitShader(moon_vs.c_str(), moon_fs.c_str());
	moon_texture_id = LoadTexture(moon_texture_name);

	// create Mars
	mars_shader_program = InitShader(mars_vs.c_str(), mars_fs.c_str());
	mars_texture_id = LoadTexture(mars_texture_name);

	// create Jupiter
	jupiter_shader_program = InitShader(jupiter_vs.c_str(), jupiter_fs.c_str());
	jupiter_texture_id = LoadTexture(jupiter_texture_name);

	// create Saturn
	saturn_shader_program = InitShader(saturn_vs.c_str(), saturn_fs.c_str());
	saturn_texture_id = LoadTexture(saturn_texture_name);
	saturn_mesh_data = LoadMesh(saturn_mesh_name);

	// create Uranus
	uranus_shader_program = InitShader(uranus_vs.c_str(), uranus_fs.c_str());
	uranus_texture_id = LoadTexture(uranus_texture_name);

	// create Neptune
	neptune_shader_program = InitShader(neptune_vs.c_str(), neptune_fs.c_str());
	neptune_texture_id = LoadTexture(neptune_texture_name);

	// create Asteroid
	asteroid_shader_program = InitShader(asteroid_vs.c_str(), asteroid_fs.c_str());
	deimos_mesh_data = LoadMesh(deimos_mesh_name);
	deimos_texture_id = LoadTexture(deimos_texture_name);
	for (int i = 0; i < 500; i++)
	{
		deimos_revolution_theta[i] = rand() % 360;
		deimos_revolution_radius[i] = rand() % 100 + 140;
		deimos_rotation[i] = rand() % 360;
		phobos_revolution_theta[i] = rand() % 360;
		phobos_revolution_radius[i] = rand() % 100 + 140;
		phobos_rotation[i] = rand() % 360;
	}
	phobos_mesh_data = LoadMesh(phobos_mesh_name);
	phobos_texture_id = LoadTexture(phobos_texture_name);

	// skybox
	cubemap_id = LoadCube(cube_name);
	cube_shader_program = InitShader(cube_vs.c_str(), cube_fs.c_str());
	cube_vao = create_cube_vao();

	// particle system
	particles_shader_program = InitShader(particles_vs.c_str(), particles_fs.c_str());

	// Initial Time
	myTime.accountingTime = 0.0;
	myTime.lastTime = 0.0;
	myTime.timeSpeed = 1.0;
	myTime.currentTime = 0.0;

	// initialize the imgui system
	ImGui_ImplGlut_Init();
}

// glut callbacks need to send keyboard and mouse events to imgui
void keyboard(unsigned char key, int x, int y)
{
	ImGui_ImplGlut_KeyCallback(key);

	switch (key)
	{
	case '1':
		SoundEngine->stopAllSounds();
		SoundEngine->play2D("audio/nier1.flac", GL_TRUE);
		break;
	case '2':
		SoundEngine->stopAllSounds();
		SoundEngine->play2D("audio/nier2.flac", GL_TRUE);
		break;
	case '3':
		SoundEngine->stopAllSounds();
		SoundEngine->play2D("audio/nier3.flac", GL_TRUE);
		break;
	case '4':
		SoundEngine->stopAllSounds();
		SoundEngine->play2D("audio/nier4.flac", GL_TRUE);
		break;
	case '5':
		SoundEngine->stopAllSounds();
		SoundEngine->play2D("audio/nier5.flac", GL_TRUE);
		break;
	case '6':
		camera = viewport1;
		break;
	case '7':
		camera = viewport2;
		break;
	case '8':
		camera = viewport3;
		break;
	case '9':
		camera = viewport4;
		break;
	case '0':
		camera = viewport0;
		break;
	case 'p':
	case 'P':
		myTime.timeSpeed = 0.0f;
		SoundEngine->play2D("audio/Pause.wav", GL_FALSE);
		break;
	case 'g':
	case 'G':
		myTime.timeSpeed = 1.0f;
		SoundEngine->play2D("audio/Go.wav", GL_FALSE);
		break;
	case '+':
		myTime.timeSpeed += 1.0f;
		SoundEngine->play2D("audio/SpeedUp.wav", GL_FALSE);
		break;
	case '-':
		if (myTime.timeSpeed >= 1.0f)
		{
			myTime.timeSpeed = 1.0f;
			SoundEngine->play2D("audio/SpeedDown.wav", GL_FALSE);
		}
		break;
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
	glEnable(GL_BLEND);
	glutInitWindowPosition(5, 5);
	glutInitWindowSize(1080, 1080);
	int win = glutCreateWindow("Solar System");
	printGlInfo();

	// BGM, realised by irrKlang
	SoundEngine->play2D("audio/nier1.flac", GL_TRUE);

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