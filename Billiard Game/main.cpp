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

#include <iostream>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "shader.hpp"
#include "model3D.hpp"

using namespace std;

#define VERTEX_SHADER_FILE		"./Resource/Shader/shader.vert"
#define FRAGMENT_SHADER_FILE	"./Resource/Shader/shader.frag"
#define MODEL_FILE				"./Resource/Model/Table_billiard N220610.3DS"
#define TEXTURE_FILE			"./Resource/Model/TextureDemo.png"

glm::mat4 projectionMarix; //in scene
Camera * camera;
Program * program;
Model3D * model;
Texture * texture;

int width = 800, height = 600;
float fovy = 45.0f; //The field of view angle, in degrees
float aspect = (float)width / (float)height;
float zNear = 0.1f, zFar = 100.0f;

//Functions prototype
void initGL(int argc, char *argv[]);
void initData();
void specialKeyFunc(int key, int x, int y);
void keyboardFunc(unsigned char key, int x, int y);
void displayFunc();

int main(int argc, char *argv[]){
	initGL(argc, argv);
	initData();

	glutDisplayFunc(displayFunc);
	glutSpecialFunc(specialKeyFunc);
	glutKeyboardFunc(keyboardFunc);

	glutMainLoop();
	return 0;
}

void initGL(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(width, height);
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
}

void initData(){
	program = new Program();
	GLuint p = program->getProgram();
	program->attachShader(new Shader(VERTEX_SHADER_FILE, GL_VERTEX_SHADER));
	program->attachShader(new Shader(FRAGMENT_SHADER_FILE, GL_FRAGMENT_SHADER));
	program->useProgram(true);

	Node::uMVPMatrixLoc = glGetUniformLocation(p, "uMVPMatrix");
	Mesh::aPositionLoc = glGetAttribLocation(p, "position");
	Mesh::aTexCoordLoc = glGetAttribLocation(p, "texCoord");
	Mesh::aNormalLoc = glGetAttribLocation(p, "normal");
	Texture::uTextureCountLoc = glGetUniformLocation(p, "texCount");
	Material::uboMaterialLoc = glGetUniformBlockIndex(p, "Material");

	projectionMarix = glm::perspective(fovy, aspect, zNear, zFar);
	camera = new Camera();
	camera->push();
	camera->translate(glm::vec3(0.0f, 0.0f, -0.34f));

	model = new Model3D();
	model->loadModel(MODEL_FILE);
	model->scale(0.0001f);
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
	case '2':
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
		break;
	}
	glutPostRedisplay();
}

void displayFunc(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program->useProgram(true);
	model->render(projectionMarix * camera->getViewMatrix());

	glutSwapBuffers();
}