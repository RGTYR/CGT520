#version 400

uniform vec4 La = vec4(0.5);
uniform vec4 Ld = vec4(1.0);
uniform vec4 Ls = vec4(0.3);
uniform vec4 ks = vec4(0.3);

uniform float alpha = 20.0; //phong specular exponent (shininess)
const vec3 l_pos = vec3(0.0, 0.0, 0.0); //world space (directional light)

in vec3 normal;   //World-space normal vector
in vec3 p;        //World-space fragment position
in vec3 eye;      //World-space eye position

uniform sampler2D diffuse_tex;

in vec2 tex_coord;

out vec4 fragcolor;

void main(void)
{
	vec3 n = normalize(normal); // unit normal vector
	vec3 v = normalize(eye - p); // unit view vector
	vec3 l = normalize(p - l_pos);
	vec3 r = reflect(-l, n); // unit reflection vector

	// fragcolor = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 tex_color = texture(diffuse_tex, tex_coord);

	//compute phong lighting in world space
	vec4 amb = tex_color * La;
	vec4 diff = tex_color * Ld * max(dot(n, l), 0.0);
	vec4 spec = ks * Ls*pow(max(dot(r, v), 0.0), alpha);

	fragcolor = amb + diff + spec;
}
