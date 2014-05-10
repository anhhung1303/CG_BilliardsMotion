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
	camera = & (sceneManager.getScene(0)->camera);
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
	case 'o': case 'O':
		Ball * ball = (Ball *) (sceneManager.getScene(0)->objects[1]);
		ball->setVelocity(glm::vec3(0.01f, 0.0f, 0.01f));
		break;
	/*case '2':
		model->translate(-yAxis * coef);
		break;
	case '4':
		model->translate(-xAxis * coef);
		break;
	case '6':
		model->translate(xAxis * coef);
		break;
	case '8':
		model->translate(yAxis * coef);
		break;
	case '5':
		model->translate(-zAxis * coef);
		break;
	case '0':
		model->translate(zAxis * coef);
		break;
	case '1':
		model->rotate(-angle, yAxis * coef);
		break;
	case '3':
		model->rotate(angle, yAxis * coef);
		break;*/
	}
	glutPostRedisplay();
}

int startX, startY;
bool start = false;
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
			start = true;
			startX = x;
			startY = y;
		}
		else if (state == GLUT_UP){
			start = false;
		}
	}
	glutPostRedisplay();
}

void processMouseMotion(int x, int y){
	static float coef = -0.0005f;
	if (start == true){
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
