#version 400            
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 VM;

uniform float time;
uniform float animate;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec2 tex_coord;  

void main(void)
{
	vec3 d = vec3(0.0, 0.0, sin(time) * animate * pos_attrib.x);
	gl_Position = P*VM*vec4(pos_attrib + d, 1.0f);
	tex_coord = tex_coord_attrib;
}