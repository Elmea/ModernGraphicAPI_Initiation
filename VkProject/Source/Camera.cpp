#include "Camera.h"
#include "App.h"

#ifdef near
#undef near;
#endif 

#ifdef far
#undef far;
#endif 

void Camera::Update(Core::Inputs cameraInputs)
{
	rotation.x -= cameraInputs.mouseDeltaY / 8;	
	rotation.y += cameraInputs.mouseDeltaX / 8;

	if (rotation.x < -90)
		rotation.x = -89;

	if (rotation.x > 90)
		rotation.x = 89;

	if (rotation.y > 360)
		rotation.y = -360;

	if (rotation.y < -360)
		rotation.y = 360;

	if (cameraInputs.forward)
	{
		position.x += speed * cosf(glm::radians(rotation.y));
		position.z += speed * sinf(glm::radians(rotation.y));
	}
	if (cameraInputs.backward)
	{
		position.x -= speed * cosf(glm::radians(rotation.y));
		position.z -= speed * sinf(glm::radians(rotation.y));
	}

	if (cameraInputs.right)
	{
		position.x += speed * cosf(glm::radians(rotation.y + 90));
		position.z += speed * sinf(glm::radians(rotation.y + 90));
	}
	if (cameraInputs.left)
	{
		position.x -= speed * cosf(glm::radians(rotation.y + 90));
		position.z -= speed * sinf(glm::radians(rotation.y + 90));
	}

	if (cameraInputs.up)
		position.y += speed;

	if (cameraInputs.down)
		position.y -= speed;
}

glm::mat4 Camera::GetView()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	direction.y = sin(glm::radians(rotation.x));
	direction.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	glm::vec3 front = glm::normalize(direction);

	return glm::lookAt(position, position + front, {0.0f, 1.0f , 0.0f});
}

glm::mat4 Camera::GetPerspective()
{
	return glm::perspective(glm::radians(45.0f), aspect, 0.1f, 10.0f);
}

void Camera::setParameters(float l, float r, float b, float t, float n, float f)
{
	far = f;
	near = n;
	right = r;
	left = l;
	top = t;
	bottom = b;
}

Camera::Camera(const unsigned int width, const unsigned int height, float _speed, float l, float r, float b, float t, float n, float f, float _fov)
{
	aspect = (float)width / (float)height;

	far = f;
	near = n;
	right = r;
	left = l;
	top = t;
	bottom = b;

	speed = _speed;

	FOV = _fov;
}