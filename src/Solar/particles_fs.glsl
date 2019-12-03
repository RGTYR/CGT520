#version 400

uniform sampler2D diffuse_tex;
uniform float life;

in vec2 tex_coord;

out vec4 fragcolor;

void main(void)
{
	// fragcolor = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 tex_color = texture(diffuse_tex, tex_coord);
	fragcolor = tex_color;
}
