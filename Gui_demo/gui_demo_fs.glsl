#version 400

uniform sampler2D diffuse_tex;

uniform float time;
uniform float fragment_color;
uniform float animate;
uniform vec3 pls_position;
uniform float La_intensity;

in vec2 tex_coord;
in vec3 normal_vec;
in vec4 world_coord;

out vec4 fragcolor;
     
void main(void)
{   
   vec4 tex_color = texture(diffuse_tex, tex_coord);

   vec3 light_colour = vec3(0.5f, 0.5f, 0.5f);
   vec3 ambient_colour = light_colour * La_intensity;

   vec3 nw = normalize(normal_vec);
   vec4 lw = normalize(pls_position - world_coord.xyz);

   fragcolor = tex_color * fragment_color;
   fragcolor.rgb = fragcolor.rgb * ambient_colour;
   // fragcolor.rgb = pls_position;
}




















