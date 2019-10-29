#version 430

const vec4 quad[4] = vec4[] ( vec4(-1.0, 1.0, 0.0, 1.0), vec4(-1.0, -1.0, 0.0, 1.0), vec4( 1.0, 1.0, 0.0, 1.0), vec4( 1.0, -1.0, 0.0, 1.0) );

const float PI = 3.1415926535;
const int N = 5000; //number of vertices in curve

uniform int pass = 0;
uniform float time = 0.0;

//Note: no variables declared as 'in' here
out vec2 tex_coord;

vec4 curve(float); //function to generate an interesting parametric curve

void main()
{
   if(pass == 0) //quad
   {
      gl_Position = quad[ gl_VertexID ]; //get clip space coords out of quad array
      tex_coord = 0.5*(quad[ gl_VertexID ].xy + vec2(1.0)); //generate tex coords that go from 0 to 1 across quad
   }

   if(pass == 1) //curves
   {
      const float angle = (2.0*3.1415926535*gl_VertexID)/N;
      gl_Position = curve(angle);
      tex_coord = vec2(gl_VertexID/float(N));
   }

   if(pass == 2) //particles
   {
      const float angle = (2.0*3.1415926535*gl_VertexID)/N;
      gl_Position = curve(angle);
      tex_coord = vec2(gl_VertexID/float(N));

      //set point size for the particle
      gl_PointSize = 20.0*sin(10.0*angle+time);
   }
}

vec4 curve(float angle)
{
      const float r = 0.5;
      const float a = 3.0;
      const float b = 80.0 + 2.0*sin(0.4*time);
      const float c = 3.0 + 2.0*cos(0.3*time+3.0);
      const float d = 80.0;
      
      float x = cos(a*angle) - sign(cos(b*angle))*pow(abs(cos(b*angle)), 3.0);
      float y = sin(c*angle) - sign(sin(d*angle))*pow(abs(sin(d*angle)), 3.0);
      return vec4(r*x, r*y, 0.0, 1.0);
}