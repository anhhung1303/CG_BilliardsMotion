#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

#include "util.hpp"

class Camera{
private:
	std::vector<glm::mat4> viewMatrixStack;
	std::vector<glm::mat4>::iterator cur;
public:
	Camera(const glm::mat4& viewMatrix = glm::mat4(1.0f));
	virtual ~Camera();

	void lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
	void setViewMatrix(const glm::mat4& viewMatrix);
	const glm::mat4& getViewMatrix() const;

	void translate(const glm::vec3& distance);
	void translate(float dx, float dy, float dz);
	void rotate(float angle, const glm::vec3& direction);
	void rotate(float angle, float ox, float oy, float oz);
	void zoom(float coef);

	void push();
	void pop();
	const glm::mat4& backUp(int index);
	void clear();
};
