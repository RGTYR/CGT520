#version 400
out vec4 fragcolor; //the output color for this fragment
uniform float time;

// garbage text would let the picture rendered become errors
void main(void)
{   
	float time_ = time;

	//Exercise: try to change the color to red
	fragcolor = vec4(cos(time_), sin(time_), cos(time_ * 2), 1.0);

}




















