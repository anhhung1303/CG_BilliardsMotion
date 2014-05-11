#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "devil.lib")
#pragma comment(lib, "ilu.lib")
#pragma comment(lib, "ilut.lib")
#pragma comment(lib, "assimp.lib")
#endif

#include "camera.hpp"
#include "vertex.hpp"
#include "SceneManager.hpp"
#include "ResourceManager.hpp"
#include "Constant.hpp"
#include "Ball.hpp"

#include <gl/glew.h>

#include <iostream>

using namespace std;

SceneManager sceneManager;
ResourceManager resourceManager;
Camera * camera;

void init(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(Constant::SCREEN_WIDTH, Constant::SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Billiard Game");

	GLenum res = glewInit();
	if (res != GLEW_OK){
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
	}
	printf("======================= System Info =======================\n");
	printf("\tVendor: %s\n", glGetString(GL_VENDOR));
	printf("\tRenderer: %s\n", glGetString(GL_RENDERER));
	printf("\tGL Version: %s\n", glGetString(GL_VERSION));
	printf("\tGLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("============================================================\n");

	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_FRONT);
	//glEnable(GL_CULL_FACE);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	resourceManager.load(Constant::RESOURCE_FILE);
	sceneManager.load(Constant::SCENE_FILE, & resourceManager);
	camera = sceneManager.getScene(0)->getUsingCamera();
}

void displayFunc(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneManager.renderScene(0);
	glutSwapBuffers();
}

void idleFunc(){
	glutPostRedisplay();
}

void specialKeyFunc(int key, int x, int y){
	static float coef = -0.005f; //Coefficient must to be negative
	switch (key) {
	case GLUT_KEY_UP:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL){ //Zoom in
			camera->translate(0.0f, 0.0f, -coef * 2);
		}
		else { //Move Camera up
			camera->translate(0.0f, coef, 0.0f);
		}
		break;
	case GLUT_KEY_DOWN:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL){ //Zoom out
			camera->translate(0.0f, 0.0f, coef * 2);
		}
		else { //Move Camera down
			camera->translate(0.0f, -coef, 0.0f);
		}
		break;
	case GLUT_KEY_RIGHT: //Move Camera right
		camera->translate(coef, 0.0f, 0.0f);
		break;
	case GLUT_KEY_LEFT: //Move Camera left
		camera->translate(-coef, 0.0f, 0.0f);
		break;
	case GLUT_KEY_F4:
		if (glutGetModifiers() == GLUT_ACTIVE_ALT){ //Exit
			exit(0);
		}
	}
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y){
	static float angle = 1.0f;
	static float coef = -0.005f;
	switch (key){
	case 'q': case 'Q':
		camera->rotate(-angle, xAxis);
		break;
	case 'w': case 'W':
		camera->rotate(angle, xAxis);
		break;
	case 'a': case 'A':
		camera->rotate(-angle, yAxis);
		break;
	case 's': case 'S':
		camera->rotate(angle, yAxis);
		break;
	case 'z': case 'Z':
		camera->rotate(-angle, zAxis);
		break;
	case 'x': case 'X':
		camera->rotate(angle, zAxis);
		break;
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		sceneManager.getScene(0)->setUsingCamera(key - '0');
		camera = sceneManager.getScene(0)->getUsingCamera();
		break;
	//TODO delete this case
	case 'o': case 'O':
		Ball * ball = (Ball *)(sceneManager.getScene(0)->objects[1]);
		ball->setVelocity(glm::vec3(0.02f, 0.0f, 0.02f));
		break;
	//default:
	//	break;
	}
	glutPostRedisplay();
}

int startX, startY;
bool startLeft = false;
bool startRight = false;
glm::vec4 getRay(float mousex, float mousey);
void processMouseButtons(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		cout << "Left ";
	}
	else if (button == GLUT_RIGHT_BUTTON){
		cout << "Right ";
	}
	else if (button == GLUT_MIDDLE_BUTTON){
		cout << "Middle ";
	}
	if (state == GLUT_DOWN){
		cout << "Down ";
	}
	else if (state == GLUT_UP){
		cout << "Up ";
	}
	cout << x << " " << y << endl;
	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			startLeft = true;
			startX = x;
			startY = y;
		}
		else if (state == GLUT_UP){
			startLeft = false;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON){
		if (state == GLUT_DOWN){
			startRight = true;
			startX = x;
			startY = y;
		}
		else if (state == GLUT_UP){
			startRight = false;

			glm::vec4 cameraInWorld = glm::inverse(camera->getViewMatrix()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			
			glm::vec4 firstRay = getRay(startX, startY);
			cout << "firstRay = " << firstRay.x << " " << firstRay.y << " " << firstRay.z << " " << firstRay.t << " " << endl;
			firstRay.x = firstRay.x / firstRay.y * -cameraInWorld.y;
			firstRay.z = firstRay.z / firstRay.y * -cameraInWorld.y;
			firstRay.y = -cameraInWorld.y;

			glm::vec4 secondRay = getRay(x, y);
			cout << "secondRay = " << secondRay.x << " " << secondRay.y << " " << secondRay.z << " " << secondRay.t << " " << endl;
			secondRay.x = secondRay.x / secondRay.y * -cameraInWorld.y;
			secondRay.z = secondRay.z / secondRay.y * -cameraInWorld.y;
			secondRay.y = -cameraInWorld.y;

			glm::vec3 velocity = glm::vec3(secondRay.x - firstRay.x, secondRay.y - firstRay.y, secondRay.z - firstRay.z);
			velocity = glm::normalize(velocity);
			cout << "velocity = " << velocity.x << " " << velocity.y << " " << velocity.z << " " << velocity.t << " " << endl;
			cout << "length = " << glm::length(velocity) << endl;
			
			Ball * ball = (Ball *)(sceneManager.getScene(0)->objects[1]);
			ball->setVelocity(glm::vec3(velocity.x * 0.02f, 0.0f, velocity.z * 0.02f));
		}
	}

	glutPostRedisplay();
}

glm::vec4 getRay(float mouse_x, float mouse_y){
	float x = (2.0f * mouse_x) / Constant::SCREEN_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * mouse_y) / Constant::SCREEN_HEIGHT;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);

	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);

	glm::vec4 ray_eye = glm::inverse(sceneManager.getScene(0)->projectionMarix) * ray_clip;

	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec4 ray_world4 = glm::inverse(camera->getViewMatrix()) * ray_eye;
	glm::vec3 result = glm::vec3(ray_world4.x, ray_world4.y, ray_world4.z);
	// don't forget to normalise the vector at some point
	result = glm::normalize(result);
	return glm::vec4(result.x, result.y, result.z, 0.0f);
}

void processMouseMotion(int x, int y){
	static float coef = -0.0005f;
	if (startLeft == true){
		int deltaX = startX - x;
		int deltaY = startY - y;
		//cout << "Move " << deltaX << " " << deltaY << endl;
		camera->translate(0.0f, -coef * deltaY, 0.0f);
		camera->translate(coef * deltaX, 0.0f, 0.0f);
		startX = x;
		startY = y;
	}
	glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y) {
	static float coef = -0.005f; //Coefficient must to be negative
	if (direction > 0){
		camera->translate(0.0f, 0.0f, -coef * 2);
	}
	else {
		camera->translate(0.0f, 0.0f, coef * 2);
	}
	glutPostRedisplay();
}
int main(int argc, char *argv[]){
	init(argc, argv);

	glutDisplayFunc(displayFunc);
	glutIdleFunc(idleFunc);
	glutSpecialFunc(specialKeyFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	//glutMouseWheelFunc(mouseWheel);

	glutMainLoop();

	sceneManager.unload();
	resourceManager.unload();
	return 0;
}
