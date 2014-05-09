#include "camera.hpp"

Camera::Camera(const glm::mat4& viewMatrix){
	clear();
	setViewMatrix(viewMatrix);
}

Camera::~Camera(){
	clear();
}

void Camera::setViewMatrix(const glm::mat4& viewMatrix){
	*cur = viewMatrix;
}

void Camera::lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up){
	setViewMatrix(glm::lookAt(eye, target, up));
}

const glm::mat4& Camera::getViewMatrix() const{
	return (*cur);
}

void Camera::translate(const glm::vec3& distance){
	*cur = glm::translate(distance) * (*cur);
}

void Camera::translate(float dx, float dy, float dz){
	translate(glm::vec3(dx, dy, dz));
}

void Camera::rotate(float angle, const glm::vec3& direction){
	*cur = glm::rotate(angle, direction) * (*cur);
}

void Camera::rotate(float angle, float ox, float oy, float oz){
	rotate(angle, glm::vec3(ox, oy, oz));
}

void Camera::zoom(float coef){
	*cur = glm::translate(glm::vec3(0.0f, 0.0f, coef)) * (*cur);
}

void Camera::push(){
	viewMatrixStack.erase(cur + 1, viewMatrixStack.end());
	viewMatrixStack.push_back(*cur);
	cur = viewMatrixStack.end() - 1;
}

void Camera::pop(){
	viewMatrixStack.pop_back();
	if (viewMatrixStack.empty()){
		viewMatrixStack.push_back(idMat4);
	}
	if (cur == viewMatrixStack.end()){
		cur--;
	}
}

const glm::mat4& Camera::backUp(int index){
	index = glm::clamp(index, 0, (int)viewMatrixStack.size() - 1);
	cur = viewMatrixStack.begin() + index;
	return (*cur);
}

void Camera::clear(){
	viewMatrixStack.clear();
	viewMatrixStack.push_back(idMat4);
	cur = viewMatrixStack.begin();
}