#include "camera.hpp"

Camera::Camera(glm::mat4 viewMatrix){
	setViewMatrix(viewMatrix);
}

Camera::~Camera(){
}

void Camera::setViewMatrix(glm::mat4 viewMatrix){
	this->viewMatrix = viewMatrix;
}

void Camera::lookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 up){
	this->viewMatrix = glm::lookAt(eye, centre, up);
}

const glm::mat4& Camera::getViewMatrix() const{
	return this->viewMatrix;
}

void Camera::translate(glm::vec3 distance){
	this->viewMatrix = glm::translate(this->viewMatrix, distance);
}

void Camera::translate(float dx, float dy, float dz){
	translate(glm::vec3(dx, dy, dz));
}

void Camera::rotate(float angle, glm::vec3 direction){
	this->viewMatrix = glm::rotate(this->viewMatrix, angle, direction);
}

void Camera::rotate(float angle, float ox, float oy, float oz){
	rotate(angle, glm::vec3(ox, oy, oz));
}

void Camera::zoom(float coef){
	this->viewMatrix = glm::translate(this->viewMatrix, glm::vec3(0.0f, 0.0f, coef));
}