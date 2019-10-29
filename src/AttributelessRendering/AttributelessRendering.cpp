#include <windows.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "InitShader.h"    //Functions for loading shaders from text files

static const std::string vertex_shader("attribless_vs.glsl");
static const std::string fragment_shader("attribless_fs.glsl");

GLuint shader_program = -1;
GLuint vao = -1;

int time_loc = -1;
int pass_loc = -1;

// glut display callback function.
// This function gets called every time the scene gets redisplayed
void display()
{
   //We don't need to clear the color buffer because we are drawing a fullscreen quad to clear to a gradient.
   glClear(GL_DEPTH_BUFFER_BIT);

   glDepthMask(GL_FALSE); //disable writes to depth buffer. Keep the cleared depth values when drawing the quad.
   //Pass 0: draw quad
   glUniform1i(pass_loc, 0);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // draw quad
   glDepthMask(GL_TRUE); //enable writes to depth buffer

   const int N = 5000;
   //Pass 1: draw curve
   glUniform1i(pass_loc, 1);
   glDrawArrays(GL_LINE_STRIP, 0, N);

   //Pass 2: draw particles
   glUniform1i(pass_loc, 2);
   glDrawArrays(GL_POINTS, 0, N);

   glutSwapBuffers();
   glutPostRedisplay();

   float time_sec = 0.001f*glutGet(GLUT_ELAPSED_TIME);
   //Pass time_sec value to the shaders
   glUniform1f(time_loc, time_sec);

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
      time_loc = glGetUniformLocation(shader_program, "time");
      pass_loc = glGetUniformLocation(shader_program, "pass");
   }
}

// glut keyboard callback function.
// This function gets called when an ASCII key is pressed
void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 'r':
   case 'R':
      reload_shader();
      break;
   }
}


//Print out information about the OpenGL version supported by the graphics driver.	
void printGlInfo()
{
   std::cout << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   std::cout << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   std::cout << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;
}

void initOpenGl()
{
   printGlInfo();

   glewInit();

   glEnable(GL_DEPTH_TEST);

   reload_shader();
   
   //Enable use of gl_PointSize in vertex shader
   glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

   //Enable use of gl_PointCoord in fragment shader
   glEnable(GL_POINT_SPRITE);

   //Generate a vertex array object, then bind it
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);
}

//C++ programs start executing in the main() function.
int main (int argc, char **argv)
{
   //Configure initial window state
   glutInit(&argc, argv); // can pass command line args to glut
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowPosition (5, 5);
   glutInitWindowSize (512, 512);
   int win = glutCreateWindow ("Attributeless rendering");

   //Register callback functions with glut. 
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   initOpenGl();

   //Enter the glut event loop.
   glutMainLoop();
   glutDestroyWindow(win);
   return 0;		
}

