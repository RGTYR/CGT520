#ifndef __SURF_H__
#define __SURF_H__

#include <windows.h>
#include <GL/glew.h>

GLuint create_surf_vao();
GLuint create_triangles_vao();
void draw_surf_points(GLuint vao);
void draw_surf_triangles(GLuint vao);

#endif
