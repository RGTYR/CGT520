#version 400

uniform sampler2D diffuse_tex;

uniform float time;
uniform float fragment_color;
uniform float animate;
uniform float La_intensity;
uniform float Ld_intensity;
uniform float Ls_intensity;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float shininess;
uniform bool kCheckbox;

in vec2 tex_coord;
in vec3 world_coord;
in vec3 light_coord;
in vec3 normal_vec;
in vec3 view_coord;
in float d;

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

   float spec = pow(max(dot(vw, rw), 0.0f), shininess);
   vec3 specular_colour = spec * light_colour * Ls_intensity;

   // The quadratic attenuation
   float attenuation_a = 0.0;
   float attenuation_b = 0.0;
   float attenuation_c = 1.0;
   float attenuation = attenuation_a + attenuation_b * d + attenuation_c * d * d;


   fragcolor = tex_color * fragment_color;
   if (kCheckbox)
   {
	   fragcolor.rgb = fragcolor.rgb * (ambient_colour + (diffuse_colour + specular_colour) / attenuation);
   }
   else 
   {
	   fragcolor.rgb = ka * ambient_colour + 
		   kd * diffuse_colour / attenuation + 
		   fragcolor.rgb * specular_colour / attenuation;
   }
   
}




















