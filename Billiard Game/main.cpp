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
}

void displayFunc(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneManager.renderScene(0);
	//cout << "View point = " << sceneManager.getScene(0)->getUsingCamera()->getViewPoint() << endl;
	glutSwapBuffers();
}

void idleFunc(){
	glutPostRedisplay();
}

int main(int argc, char *argv[]){
	init(argc, argv);

	glutDisplayFunc(displayFunc);
	glutIdleFunc(idleFunc);
	sceneManager.getScene(0)->setUsingCamera(0);
	Keyboard::setCamera(sceneManager.getScene(0)->getUsingCamera());
	Keyboard::setSceneManager(&sceneManager);
	glutSpecialFunc(Keyboard::specialKeyFunc);
	glutKeyboardFunc(Keyboard::keyboardFunc);

	Mouse::setCamera(sceneManager.getScene(0)->getUsingCamera());
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