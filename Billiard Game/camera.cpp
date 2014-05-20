#include "camera.hpp"

GLuint Camera::uViewMatrixLoc = 2;
GLuint Camera::uViewPosLoc = 3;
GLuint Camera::uProjMatrixLoc = -1;

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

glm::vec3 Camera::getViewPoint() const{
	return glm::vec3(glm::inverse(*cur) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

glm::mat4 Camera::getProjectionMatrix(){
	return projectionMatrix;
}

void Camera::setProjectionMatrix(glm::mat4 newProjectionMatrix){
	projectionMatrix = newProjectionMatrix;
}

void Camera::translate(glm::vec3 distance, const Coordinates& coor, const Coordinates& dis){
	switch (dis){
	case WORLD_COORDINATES:
		if (coor == VIEW_COORDINATES){ //Convert distance from world coordiantes to view coordiantes
			distance = extractScaling(*cur) * distance;
		}
		break;
	case VIEW_COORDINATES:
		if (coor == WORLD_COORDINATES){ //Convert distance form view coordiante to world coordiante
			distance = (1.0f / extractScaling(*cur)) * distance;
		}
		break;
	default:
		return; //Model Coordiante Invalid enum => Do nothing
	}

	switch (coor)	{
	case WORLD_COORDINATES:
		*cur = glm::translate((*cur), -distance);
		return;
	case VIEW_COORDINATES:
		*cur = glm::translate(-distance) * (*cur);
		return;
	}
	//Model Coordinate: Invalid enum => Do nothing
}

void Camera::translate(float dx, float dy, float dz, const Coordinates& coor, const Coordinates& dis){
	this->translate(glm::vec3(dx, dy, dz), coor, dis);
}

void Camera::rotate(float angle, const glm::vec3& orientation, const Coordinates& coor){
	switch (coor){
	case WORLD_COORDINATES:
		*cur = glm::rotate((*cur), -angle, orientation);
		return;
	case VIEW_COORDINATES:
		*cur = glm::rotate(-angle, orientation) * (*cur);
		return;
	}
	//Model Coordinate: Invalid enum => Do nothing
}

void Camera::rotate(float angle, float ox, float oy, float oz, const Coordinates& coor){
	this->rotate(angle, glm::vec3(ox, oy, oz), coor);
}

void Camera::scale(const glm::vec3& factor, const Coordinates& coor){
	switch (coor)	{
	case WORLD_COORDINATES:
		*cur = glm::scale((*cur), 1.0f / factor);
		return;
	case VIEW_COORDINATES:
		*cur = glm::scale(1.0f / factor) * (*cur);
		return;
	}
	//Model Coordinate: Invalid enum => Do nothing
}

void Camera::scale(float factor, const Coordinates& coor){
	this->scale(glm::vec3(factor), coor);
}

void Camera::push(){
	viewMatrixStack.erase(cur + 1, viewMatrixStack.end());
	viewMatrixStack.push_back(*cur);
	cur = viewMatrixStack.end() - 1;
}

void Camera::pop(){
	if (viewMatrixStack.size() == 1){
		setViewMatrix(idMat4);
		return;
	}
	if (cur == viewMatrixStack.end() - 1){ //The last element in stack
		cur--;
	}
	viewMatrixStack.pop_back();
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