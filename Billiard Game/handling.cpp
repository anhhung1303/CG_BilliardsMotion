#include "handling.hpp"


float Keyboard::coef = 0.05f;
Camera * Keyboard::cam = NULL;
SceneManager * Keyboard::sceneManager = NULL;
Camera* Mouse::cam = NULL;
glm::ivec2 Mouse::dragStartPos = glm::ivec2(-1);
int Mouse::windowWidth = 800, Mouse::windowHeight = 600;
const float Mouse::coef = 90.0f;

bool Mouse::isLeftDown = false;
bool Mouse::isRightDown = false;
int Mouse::startRightX = 0;
int Mouse::startRightY = 0;
Ball * Mouse::ball = NULL;

void Keyboard::setCamera(Camera* cam){
	Keyboard::cam = cam;
}

void Keyboard::setSceneManager(SceneManager * newSceneManager){
	Keyboard::sceneManager = newSceneManager;
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
	cout << "View point = " << cam->getViewPoint() << endl;
	glutPostRedisplay();
}

void Keyboard::keyboardFunc(unsigned char key, int x, int y){
	//Do something here
	switch (key){
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		Keyboard::sceneManager->getScene(0)->setUsingCamera(key - '0');
		Keyboard::cam = Keyboard::sceneManager->getScene(0)->getUsingCamera();
		Mouse::setCamera(Keyboard::cam);
		break;
	default:
		return;
	}
	glutPostRedisplay();
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
			isLeftDown = true;
		}
		else { //Left Mouse is release
			isLeftDown = false;
			cam->pop();
			applyChange(glm::ivec2(x, y));

			dragStartPos = glm::ivec2(-1);
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN){
			isRightDown = true;
			startRightX = x;
			startRightY = y;
		}
		else {//Right mouse is release
			isRightDown = false;

			glm::vec4 cameraInWorld = glm::inverse(cam->getViewMatrix()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			glm::vec4 firstRay = getRay(startRightX, startRightY);
			firstRay.x = firstRay.x / firstRay.y * -cameraInWorld.y;
			firstRay.z = firstRay.z / firstRay.y * -cameraInWorld.y;
			firstRay.y = -cameraInWorld.y;
			//cout << "firstRay = " << firstRay.x << " " << firstRay.y << " " << firstRay.z << " " << firstRay.w << " " << endl;

			glm::vec4 secondRay = getRay(x, y);
			secondRay.x = secondRay.x / secondRay.y * -cameraInWorld.y;
			secondRay.z = secondRay.z / secondRay.y * -cameraInWorld.y;
			secondRay.y = -cameraInWorld.y;
			//cout << "secondRay = " << secondRay.x << " " << secondRay.y << " " << secondRay.z << " " << secondRay.w << " " << endl;

			glm::vec3 velocity = glm::vec3(secondRay.x - firstRay.x, secondRay.y - firstRay.y, secondRay.z - firstRay.z);
			if (glm::length(velocity) > 0){
				velocity = glm::normalize(velocity);
				cout << "velocity = " << velocity.x << " " << velocity.y << " " << velocity.z << " " << endl;
				cout << "length = " << glm::length(velocity) << endl;

				ball->setVelocity(glm::vec3(velocity.x * 0.02f, 0.0f, velocity.z * 0.02f));
			}
		}
		break;
	}
}

glm::vec4 Mouse::getRay(float mouse_x, float mouse_y){
	float x = (2.0f * mouse_x) / Constant::SCREEN_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / Constant::SCREEN_HEIGHT;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

	glm::vec4 ray_eye = glm::inverse(cam->getProjectionMatrix()) * ray_clip;

	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec4 ray_world4 = glm::inverse(cam->getViewMatrix()) * ray_eye;
	glm::vec3 result = glm::vec3(ray_world4.x, ray_world4.y, ray_world4.z);
	// don't forget to normalise the vector at some point
	result = glm::normalize(result);
	return glm::vec4(result.x, result.y, result.z, 0.0f);
}

void Mouse::mouseMotionFunc(int x, int y){
	if (isLeftDown){
		cam->pop(); //Remove old matrix
		cam->push();
		applyChange(glm::ivec2(x, y));
		glutPostRedisplay();
	}
	else if (isRightDown){
		//Do something
	}
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

void Mouse::setControllObject(Ball * newBall){
	ball = newBall;
}