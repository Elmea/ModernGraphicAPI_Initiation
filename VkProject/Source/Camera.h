#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
	struct Inputs;
}

class Camera
{
#ifdef near
#undef near;
#endif 

#ifdef far
#undef far;
#endif 

private:
	glm::vec3 position = { 2.f, 0.5f , 0.f };
	glm::vec3 rotation = { 0.f, 180.f , 0.f };

	float right, left;
	float top, bottom;
	float far, near;
	float FOV, aspect;

public:
	Camera(const unsigned int width, const unsigned int height, float _speed, float l, float r, float b, float t, float n, float f, float _fov);
	void Update(Core::Inputs inputs);
	void setParameters(float l, float r, float b, float t, float n, float f);

	float speed = 0.1f;

	glm::mat4 GetView();
	glm::mat4 GetPerspective();
};