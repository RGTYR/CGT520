// camera.h
#ifndef __CAMERA_H__
#define __CAMERA_H__


#include <iostream>
#include <glm/glm.hpp>

enum MoveDirection
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	FORWARD,
	BACKWARD
};

class Camera
{
public:
	
	// Position of Camera
	glm::vec3 cameraPos;
	// The direction of Camera
	glm::vec3 cameraTarget;
	// The up vector of the world
	glm::vec3 worldUp;

	// Euler Angle
	float pitch;
	float yaw;
	float roll;

	float moveSpeed;

	void move(MoveDirection moveDirection);
	glm::vec3 cameraDir(int x, int y, float sensitivity);

	Camera(glm::vec3 pos = glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	~Camera();

private:
	float lastX;
	float lastY;
	bool InitMouseDir;
};

#endif // !__CAMERA_H__
