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

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "camera.hpp"
#include "vertex.hpp"
#include "SceneManager.hpp"
#include "ResourceManager.hpp"
#include "Constant.hpp"
#include "Ball.hpp"
#include "handling.hpp"


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
	fprintf(stdout, "======================= System Info =======================\n");
	fprintf(stdout, "\tVendor: %s\n", glGetString(GL_VENDOR));
	fprintf(stdout, "\tRenderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, "\tGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "\tGLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(stdout, "\tGLEW Version: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "===========================================================\n");

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POLYGON_OFFSET_LINE);
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

void keyboardFunc(unsigned char key, int x, int y){
	static float angle = 1.0f;
	static float coef = -0.005f;
	switch (key){
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		sceneManager.getScene(0)->setUsingCamera(key - '0');
		camera = sceneManager.getScene(0)->getUsingCamera();
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
			firstRay.x = firstRay.x / firstRay.y * -cameraInWorld.y;
			firstRay.z = firstRay.z / firstRay.y * -cameraInWorld.y;
			firstRay.y = -cameraInWorld.y;
			cout << "firstRay = " << firstRay.x << " " << firstRay.y << " " << firstRay.z << " " << firstRay.w << " " << endl;

			glm::vec4 secondRay = getRay(x, y);
			secondRay.x = secondRay.x / secondRay.y * -cameraInWorld.y;
			secondRay.z = secondRay.z / secondRay.y * -cameraInWorld.y;
			secondRay.y = -cameraInWorld.y;
			cout << "secondRay = " << secondRay.x << " " << secondRay.y << " " << secondRay.z << " " << secondRay.w << " " << endl;

			glm::vec3 velocity = glm::vec3(secondRay.x - firstRay.x, secondRay.y - firstRay.y, secondRay.z - firstRay.z);
			if (glm::length(velocity) > 0){
				velocity = glm::normalize(velocity);
				cout << "velocity = " << velocity.x << " " << velocity.y << " " << velocity.z << " " << endl;
				cout << "length = " << glm::length(velocity) << endl;

				Ball * ball = (Ball *)(sceneManager.getScene(0)->objects[1]);
				ball->setVelocity(glm::vec3(velocity.x * 0.02f, 0.0f, velocity.z * 0.02f));
			}
			
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

	glm::vec4 ray_eye = glm::inverse(camera->getProjectionMatrix()) * ray_clip;

	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec4 ray_world4 = glm::inverse(camera->getViewMatrix()) * ray_eye;
	glm::vec3 result = glm::vec3(ray_world4.x, ray_world4.y, ray_world4.z);
	// don't forget to normalise the vector at some point
	result = glm::normalize(result);
	return glm::vec4(result.x, result.y, result.z, 0.0f);
}

//void mouseWheel(int wheel, int direction, int x, int y) {
//	static float coef = -0.005f; //Coefficient must to be negative
//	if (direction > 0){
//		camera->translate(0.0f, 0.0f, -coef * 2);
//	}
//	else {
//		camera->translate(0.0f, 0.0f, coef * 2);
//	}
//	glutPostRedisplay();
//}

int main(int argc, char *argv[]){
	init(argc, argv);

	glutDisplayFunc(displayFunc);
	glutIdleFunc(idleFunc);
	Keyboard::setCamera(camera);
	glutSpecialFunc(Keyboard::specialKeyFunc);
	glutKeyboardFunc(Keyboard::keyboardFunc);

	Mouse::setCamera(camera);
	Mouse::setWindow(Constant::SCREEN_WIDTH, Constant::SCREEN_HEIGHT);
	Mouse::setControllObject((Ball *)(sceneManager.getScene(0)->objects[1]));
	glutMouseFunc(Mouse::mouseFunc);
	glutMotionFunc(Mouse::mouseMotionFunc);
	//glutMouseWheelFunc(mouseWheel);

	glutMainLoop();

	sceneManager.unload();
	resourceManager.unload();

	return 0;
}