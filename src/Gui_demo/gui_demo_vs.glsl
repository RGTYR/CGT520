#version 400
uniform mat4 PVM;
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float time;
uniform float animate;
uniform vec3 pls_position;
uniform vec3 view_position;

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec2 tex_coord;
out vec3 world_coord;
out vec3 light_coord;
out vec3 normal_vec;
out vec3 view_coord;
out float d;


void main(void)
{
	vec3 animation = vec3(0.0, 0.0, sin(time) * animate * pos_attrib.x);

	gl_Position = P * (V*M)*vec4(pos_attrib + animation, 1.0f);
	tex_coord = tex_coord_attrib;

	d = distance(pls_position, pos_attrib);

	// convert
	// light vector, object vectorm, normal vector, view vector
	// into world vectors
	world_coord = vec3(M * vec4(pos_attrib, 1.0f));
	light_coord = vec3(M * vec4(pls_position, 1.0f));
	// vec3 light_coord = pls_position;
	normal_vec = mat3(transpose(inverse(M))) * normal_attrib;
	view_coord = vec3(M * vec4(view_position, 1.0f));

}