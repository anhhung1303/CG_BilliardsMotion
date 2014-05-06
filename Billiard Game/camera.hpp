#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
	Camera(glm::mat4 viewMatrix = glm::mat4(1.0f));
	virtual ~Camera();
	
	void setViewMatrix(glm::mat4 viewMatrix);
	void lookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 up);
	const glm::mat4& getViewMatrix() const;

	void translate(glm::vec2 distance);
	void rotate(float angle, glm::vec2 direction);
	void zoom(float coef);
private:
	glm::mat4 viewMatrix;
};