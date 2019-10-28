#version 400

uniform sampler2D diffuse_tex;

uniform float time;
uniform float fragment_color;
uniform float animate;
uniform float La_intensity;
uniform float Ld_intensity;
uniform float Ls_intensity;

in vec2 tex_coord;
in vec3 world_coord;
in vec3 light_coord;
in vec3 normal_vec;
in vec3 view_coord;

out vec4 fragcolor;
     
void main(void)
{   
   vec4 tex_color = texture(diffuse_tex, tex_coord);

   vec3 lw = normalize(world_coord - light_coord);
   vec3 nw = normalize(normal_vec);
   vec3 vw = normalize(view_coord - world_coord);
   vec3 rw = reflect(-lw, nw);

   vec3 light_colour = vec3(0.5f, 0.5f, 0.5f);
   vec3 ambient_colour = light_colour * La_intensity;

   float diff = max(dot(nw, lw), 0.0);
   vec3 diffuse_colour = diff * light_colour * Ld_intensity;

   int shininess = 32;
   float spec = pow(max(dot(vw, rw), 0.0f), shininess);
   vec3 specular_colour = spec * light_colour * Ls_intensity;

   fragcolor = tex_color * fragment_color;
   fragcolor.rgb = fragcolor.rgb * (ambient_colour + diffuse_colour + specular_colour);
}




















