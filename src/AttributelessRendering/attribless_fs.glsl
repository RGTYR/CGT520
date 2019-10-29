#version 430

uniform int pass = 0;
uniform float time = 0.0;

in vec2 tex_coord;

out vec4 fragcolor;

const vec4 grad1 = vec4(1.0);
const vec4 grad2 = vec4(0.1);

void main(void)
{
   if(pass == 0) //color gradient for the quad
   {
      fragcolor = mix(grad1, grad2, smoothstep(0.2, 0.8, tex_coord.y));
   }

   if(pass == 1) //vary the color along the curve
   {
      const float c = 0.5*sin(12.0*tex_coord.x)+0.5;
      fragcolor = vec4(c, c, c, 1.0);
   }

   if(pass == 2) //color for each particle
   {
      //Use gl_PointCoord to make rounded particles
      const float r = distance(vec2(0.5), gl_PointCoord.xy);
      if(r > 0.5)
      {
         discard; //throw away fragments outside of a circle
      }

      const float c = 0.5*sin(12.0*tex_coord.x)+0.5;
      fragcolor = vec4(1.0-c, 1.0-c, 1.0-c, 1.0);
   }
}