#include "handling.hpp"


float Keyboard::coef = 0.05f;
Camera* Keyboard::cam = NULL;
Camera* Mouse::cam = NULL;
glm::ivec2 Mouse::dragStartPos = glm::ivec2(-1);
int Mouse::windowWidth = 800, Mouse::windowHeight = 600;
const float Mouse::coef = 90.0f;

void Keyboard::setCamera(Camera* cam){
	Keyboard::cam = cam;
}

void Keyboard::specialKeyFunc(int key, int x, int y){
	switch (key) {
	case GLUT_KEY_UP:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL){ //Move camera frontward
			cam->translate(0.0f, 0.0f, -coef * 2, VIEW_COORDINATES, VIEW_COORDINATES);
		}
		else { //Move Camera up
			cam->translate(0.0f, coef, 0.0f, VIEW_COORDINATES, VIEW_COORDINATES);
		}
		break;
	case GLUT_KEY_DOWN:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL){ //Move camera backward
			cam->translate(0.0f, 0.0f, coef * 2, VIEW_COORDINATES, VIEW_COORDINATES);
		}
		else { //Move Camera down
			cam->translate(0.0f, -coef, 0.0f, VIEW_COORDINATES, VIEW_COORDINATES);
		}
		break;
	case GLUT_KEY_RIGHT: //Move Camera right
		cam->translate(coef, 0.0f, 0.0f, VIEW_COORDINATES, VIEW_COORDINATES);
		break;
	case GLUT_KEY_LEFT: //Move Camera left
		cam->translate(-coef, 0.0f, 0.0f, VIEW_COORDINATES, VIEW_COORDINATES);
		break;
	case GLUT_KEY_F4:
		if (glutGetModifiers() == GLUT_ACTIVE_ALT){ //Exit
			exit(EXIT_SUCCESS);
		}
	default:
		return;
	}
	glutPostRedisplay();
}

void Keyboard::keyboardFunc(unsigned char key, int x, int y){
	//Do something here
}

void Mouse::setCamera(Camera* cam){
	Mouse::cam = cam;
}

void Mouse::setWindow(int width, int height){
	Mouse::windowWidth = width;
	Mouse::windowHeight = height;
}

void Mouse::mouseFunc(int button, int state, int x, int y){
	switch (button){
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN){ //Left Mouse is press
			dragStartPos = glm::ivec2(x, y);
			cam->push();
		}
		else { //Left Mouse is release
			cam->pop();
			applyChange(glm::ivec2(x, y));

			dragStartPos = glm::ivec2(-1);
			glutPostRedisplay();
		}
	}
}

void Mouse::mouseMotionFunc(int x, int y){
	cam->pop(); //Remove old matrix
	cam->push();
	applyChange(glm::ivec2(x, y));
	glutPostRedisplay();
}

void Mouse::applyChange(const glm::ivec2& curPos){
	glm::ivec2 delta = dragStartPos - curPos;

	glm::mat3 viewInverse = glm::inverse(glm::mat3(cam->getViewMatrix()));
	glm::vec3 vAxis = glm::normalize(viewInverse * yAxis);
	cam->rotate(coef * delta.x / float(windowWidth), vAxis, WORLD_COORDINATES);
	//cam->rotate(coef * delta.x / float(windowWidth), yAxis, WORLD_COORDINATES);

	viewInverse = glm::inverse(glm::mat3(cam->getViewMatrix()));
	glm::vec3 uAxis = glm::normalize(viewInverse * xAxis);
	cam->rotate(coef * delta.y / float(windowHeight), uAxis, WORLD_COORDINATES);
}

void Mouse::mouseWheelFunc(int wheel, int direction, int x, int y){
	if (direction > 0){ //Move camera frontward
		cam->translate(0.0f, 0.0f, -coef * 2, VIEW_COORDINATES, VIEW_COORDINATES);
	}
	else { //Move camera backward
		cam->translate(0.0f, 0.0f, coef * 2, VIEW_COORDINATES, VIEW_COORDINATES);
	}
}