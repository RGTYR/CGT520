#include <windows.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <iostream>

// glut display callback function.
// This function gets called every time the scene gets redisplayed
void display()
{
   //Clear the screen to the color previously specified in the glClearColor(...) call.
   glClear(GL_COLOR_BUFFER_BIT);

   //Here is where you would write code to draw something.

   glFinish();
}

// glut keyboard callback function.
// This function gets called when an ASCII key is pressed
void keyboard(unsigned char key, int x, int y)
{
	std::cout << "key : " << key << ", x: " << x << ", y: " << y << std::endl;

	switch (key)
	{
		case '+':
			glutReshapeWindow(glutGet(GLUT_WINDOW_WIDTH) + 10, glutGet(GLUT_WINDOW_HEIGHT) + 10);
			std::cout << "Windows Width: "	<< glutGet(GLUT_WINDOW_WIDTH) + 10	<< std::endl;
			std::cout << "Windows Height: " << glutGet(GLUT_WINDOW_HEIGHT) + 10	<< std::endl;
			break;

		case '-':
			glutReshapeWindow(glutGet(GLUT_WINDOW_WIDTH) - 10, glutGet(GLUT_WINDOW_HEIGHT) - 10);
			std::cout << "Windows Width: "	<< glutGet(GLUT_WINDOW_WIDTH) - 10	<< std::endl;
			std::cout << "Windows Height: " << glutGet(GLUT_WINDOW_HEIGHT) - 10	<< std::endl;
			break;

		case 'f':
			if (glutGet(GLUT_WINDOW_WIDTH) != (GLUT_SCREEN_WIDTH) && glutGet(GLUT_WINDOW_HEIGHT) != glutGet(GLUT_SCREEN_HEIGHT))
			{
				glutFullScreen();
			}
			else
			{
				glutReshapeWindow(glutGet(GLUT_INIT_WINDOW_WIDTH), glutGet(GLUT_INIT_WINDOW_HEIGHT));
			}
			break;

		default:
			break;
	}

	if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
	{
		std::cout << "Shift active" << std::endl;
	}
	else
	{
		std::cout << "Shift inactive" << std::endl;
	}
}

// glut keyboard callback function.
// This function gets called when an ASCII key is released
void keyboardUp(unsigned char key, int x, int y)
{
	std::cout << "RELEASED" << std::endl;
}

// glut special keyboard call back function
// This function gets called when a special button "F1, F2..." is pressed
// The list of special buttons: https://www.opengl.org/resources/libraries/glut/spec3/node54.html#SECTION00089000000000000000
void specialKeyboard(int key, int x, int y)
{
	int border_x = glutGet(GLUT_WINDOW_BORDER_WIDTH);
	int border_y = glutGet(GLUT_WINDOW_BORDER_HEIGHT);
	int position_x = glutGet(GLUT_WINDOW_X) - border_x;
	int position_y = glutGet(GLUT_WINDOW_Y) - border_y;

	std::cout << "PRESSED" << std::endl;

	switch (key)
	{
	
		case GLUT_KEY_LEFT:
			glutPositionWindow(position_x - 1, position_y);
			std::cout << "x: " << glutGet(GLUT_WINDOW_X) << "\ty: " << glutGet(GLUT_WINDOW_Y) << std::endl;
			break;

		case GLUT_KEY_RIGHT:
			glutPositionWindow(position_x + 1, position_y);
			std::cout << "x: " << glutGet(GLUT_WINDOW_X) << "\ty: " << glutGet(GLUT_WINDOW_Y) << std::endl;
			break;

		case GLUT_KEY_UP:
			glutPositionWindow(position_x, position_y - 1);
			std::cout << "x: " << glutGet(GLUT_WINDOW_X) << "\ty: " << glutGet(GLUT_WINDOW_Y) << std::endl;
			break;

		case GLUT_KEY_DOWN:
			glutPositionWindow(position_x, position_y + 1);
			std::cout << "x: " << glutGet(GLUT_WINDOW_X) << "\ty: " << glutGet(GLUT_WINDOW_Y) << std::endl;
			break;

		case GLUT_KEY_F1:
			glutHideWindow();
			break;

		case GLUT_KEY_F5:
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
			break;

		case GLUT_KEY_F6:
			glutSetCursor(GLUT_CURSOR_CYCLE);
			break;

		case GLUT_KEY_F7:
			glutSetCursor(GLUT_CURSOR_INFO);
			break;

		case GLUT_KEY_F8:
			glutSetCursor(GLUT_CURSOR_DESTROY);
			break;

		default:
			break;
	}
}

// glut special keyboard call back function
// This function gets called when a special button "F1, F2..." is released
// The list of special buttons: https://www.opengl.org/resources/libraries/glut/spec3/node54.html#SECTION00089000000000000000
void specialKeyboardUp(int key, int x, int y)
{
	std::cout << "RELEASED" << std::endl;
}

// glut mouse callback function.
// This function gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
	std::cout << "button : "<< button << ", state: " << state << ", x: " << x << ", y: " << y << std::endl;
}

// glut mouse wheel callback function.
// This function gets called when a mouse wheel is scrolled
void mouseWheel(int wheel, int dir, int x, int y)
{
	if(dir > 0)
	{
		glutReshapeWindow(glutGet(GLUT_WINDOW_WIDTH) + 10, glutGet(GLUT_WINDOW_HEIGHT) + 10);
		std::cout << "Windows Width: " << glutGet(GLUT_WINDOW_WIDTH) + 10 << std::endl;
		std::cout << "Windows Height: " << glutGet(GLUT_WINDOW_HEIGHT) + 10 << std::endl;
	}
	else if (dir < 0)
	{
		glutReshapeWindow(glutGet(GLUT_WINDOW_WIDTH) - 10, glutGet(GLUT_WINDOW_HEIGHT) - 10);
		std::cout << "Windows Width: " << glutGet(GLUT_WINDOW_WIDTH) - 10 << std::endl;
		std::cout << "Windows Height: " << glutGet(GLUT_WINDOW_HEIGHT) - 10 << std::endl;
	}
}

void changeBackground(int value)
{
	std::cout << "Change background colour into black.\n";
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glFinish();
	glutTimerFunc(5 * 1000, changeBackground, 0);
}

void reshape(int width, int height)
{
	std::cout << "Windows Width: "	<< width	<< std::endl;
	std::cout << "Windows Height: " << height	<< std::endl;
}

//Print out information about the OpenGL version supported by the graphics driver.	
void printGlInfo()
{
	std::cout << "Vendor: "						<< glGetString(GL_VENDOR)							<< std::endl;
	std::cout << "Renderer: "					<< glGetString(GL_RENDERER)							<< std::endl;
	std::cout << "Version: "					<< glGetString(GL_VERSION)							<< std::endl;
	std::cout << "GLSL Version: "				<< glGetString(GL_SHADING_LANGUAGE_VERSION)			<< std::endl;
	std::cout << "Screen Width: "				<< glutGet(GLUT_SCREEN_WIDTH)						<< std::endl;
	std::cout << "Screen Height: "				<< glutGet(GLUT_SCREEN_HEIGHT)						<< std::endl;
	std::cout << "Windows Width: "				<< glutGet(GLUT_WINDOW_WIDTH)						<< std::endl;
	std::cout << "Windows Height: "				<< glutGet(GLUT_WINDOW_HEIGHT)						<< std::endl;
	std::cout << "Numerber of bits (Red): "		<< glutGet(GLUT_WINDOW_RED_SIZE)					<< std::endl;
	std::cout << "Numerber of bits (Green): "	<< glutGet(GLUT_WINDOW_GREEN_SIZE)					<< std::endl;
	std::cout << "Numerber of bits (Blue): "	<< glutGet(GLUT_WINDOW_BLUE_SIZE)					<< std::endl;
	std::cout << "Numerber of bits (Alpha): "	<< glutGet(GLUT_WINDOW_ALPHA_SIZE)					<< std::endl;
}

//C++ programs start executing in the main() function.
int main (int argc, char **argv)
{
	//Configure initial window state
	glutInit(&argc, argv); // can pass command line args to glut
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition (5, 5);
	glutInitWindowSize (512, 512);
	int win = glutCreateWindow ("CGT 520 System Info (Junjie Luo)");

	printGlInfo();

	//Register callback functions with glut. 
	glutDisplayFunc(display); 
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKeyboard);
	glutSpecialUpFunc(specialKeyboardUp);
	glutTimerFunc(5 * 1000, changeBackground, 0);
	glutReshapeFunc(reshape);

	//Set the color the screen gets cleared to.
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	//Enter the glut event loop.
	glutMainLoop();
	glutDestroyWindow(win);
	return 0;		
}
