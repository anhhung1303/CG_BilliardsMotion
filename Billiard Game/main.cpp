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

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	resourceManager.load(Constant::RESOURCE_FILE);
	sceneManager.load(Constant::SCENE_FILE, & resourceManager);
	camera = & (sceneManager.scenes[0].camera);
}

void displayFunc(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneManager.renderScene(0);
	glutSwapBuffers();
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
int main(int argc, char *argv[]){
	init(argc, argv);

	glutDisplayFunc(displayFunc);
	glutSpecialFunc(specialKeyFunc);
	glutKeyboardFunc(keyboardFunc);

	glutMainLoop();

	sceneManager.unload();
	resourceManager.unload();
	return 0;
}
