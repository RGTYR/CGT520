#version 400
out vec4 fragcolor; //the output color for this fragment
uniform float time;

in vec3 n;

// garbage text would let the picture rendered become errors
void main(void)
{   
	float time_ = time;
	vec3 unit_normal = normalize(n);

	// Exercise: try to change the color to red
	fragcolor = vec4(unit_normal * cos(time_), 1.0);

}




















