#include "Camera.h"

void Camera::move(MoveDirection moveDirection)
{
	switch (moveDirection)
	{
	case LEFT:
		cameraPos.x -= moveSpeed;
		break;
	case RIGHT:
		cameraPos.x += moveSpeed;
		break;
	case UP:
		cameraPos.y += moveSpeed;
		break;
	case DOWN:
		cameraPos.y -= moveSpeed;
		break;
	case FORWARD:
		cameraPos.z -= moveSpeed;
		break;
	case BACKWARD:
		cameraPos.z += moveSpeed;
		break;
	default:
		break;
	}
}

glm::vec3 Camera::cameraDir(int x, int y, float sensitivity)
{
	glm::vec3 camera_dir;

	if (InitMouseDir)
	{
		lastX = x;
		lastY = y;
		InitMouseDir = false;
	}

	float xOffset = lastX - x;
	float yOffset = lastY - y;
	lastX = x;
	lastY = y;

	yaw += xOffset * sensitivity;
	pitch += yOffset * sensitivity;

	camera_dir.x = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_dir.y = sin(glm::radians(pitch));
	camera_dir.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));

	return camera_dir;
}

Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
	// std::cout << "Camera has been constructed\n";
	cameraPos = pos;
	cameraTarget = target;
	worldUp = up;

	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;

	moveSpeed = 0.5f;

	InitMouseDir = true;

	std::cout << "Camera is located at (" << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
	std::cout << "Target of Camera is located at (" << target.x << ", " << target.y << ", " << target.z << ")\n";
}

Camera::~Camera()
{
}
