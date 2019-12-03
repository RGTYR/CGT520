#version 400            
uniform mat4 PVM;

in vec3 pos_attrib; //in object space
in vec2 tex_coord_attrib;

out vec2 tex_coord;

void main(void)
{
   //Compute clip-space vertex position
   gl_Position = PVM*vec4(pos_attrib, 1.0);     //w = 1 becase this is a point
   // gl_PointSize = 5.0f;
   tex_coord = tex_coord_attrib;
}