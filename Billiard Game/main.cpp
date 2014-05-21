#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "devil.lib")
#pragma comment(lib, "ilu.lib")
#pragma comment(lib, "ilut.lib")
#pragma comment(lib, "assimp.lib")
#endif

//Third-party include
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

//System include
#include <iostream>

//Project include
#include "camera.hpp"
#include "shader.hpp"
#include "model3D.hpp"
#include "light.hpp"
#include "handling.hpp"
#include "util.hpp"

using namespace std;

#define VERTEX_SHADER_FILE		"./Resource/Shader/test.vert"
#define FRAGMENT_SHADER_FILE	"./Resource/Shader/test.frag"
#define MODEL_FILE				"./Resource/Test Model/Utah Teapot Hires.obj"
#define TEXTURE_FILE			"./Resource/Model/TextureDemo.png"

glm::mat4 projectionMarix; //in scene
GLuint uProjMatrixLoc = 1;
Camera * camera;
Program * program;
Model3D * model;
Texture * texture;
Light * light;

int width = 800, height = 600;
float fovy = 45.0f; //The field of view angle, in degrees
float aspect = (float)width / (float)height;
float zNear = 0.1f, zFar = 100.0f;

//Functions prototype
void initGL(int argc, char *argv[]);
void initData();
void displayFunc();
void drawGroundGrid(float centerX, float centerZ, float rangeX, float rangeZ, float step);

int main(int argc, char *argv[]){
	initGL(argc, argv);
	initData();

	glutDisplayFunc(displayFunc);

	Keyboard::setCamera(camera);
	glutSpecialFunc(Keyboard::specialKeyFunc);
	glutKeyboardFunc(Keyboard::keyboardFunc);

	Mouse::setCamera(camera);
	Mouse::setWindow(width, height);
	glutMouseFunc(Mouse::mouseFunc);
	glutMotionFunc(Mouse::mouseMotionFunc);
	//glutMouseWheelFunc(Mouse::mouseWheelFunc);

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
}

void initData(){
	program = new Program();
	GLuint p = program->getProgram();
	program->attachShader(new Shader(VERTEX_SHADER_FILE, GL_VERTEX_SHADER));
	program->attachShader(new Shader(FRAGMENT_SHADER_FILE, GL_FRAGMENT_SHADER));
	Program::useProgram(program);

	Mesh::aPositionLoc = glGetAttribLocation(p, "position");
	Mesh::aTexCoordLoc = glGetAttribLocation(p, "texCoord");
	Mesh::aNormalLoc = glGetAttribLocation(p, "normal");

	Node::uModelMatrixLoc = glGetUniformLocation(p, "modelMatrix");
	Camera::uViewMatrixLoc = glGetUniformLocation(p, "viewMatrix");
	uProjMatrixLoc = glGetUniformLocation(p, "projMatrix");

	Material::uboMaterialLoc = glGetUniformBlockIndex(p, "Material");
	Light::uboLightLoc = glGetUniformBlockIndex(p, "Light");
	Texture::uTextureCountLoc = glGetUniformLocation(p, "texCount");
	Camera::uViewPosLoc = glGetUniformLocation(p, "viewPos");
#ifdef PRINT_UNIFORM
	program->printActiveUniform();
#endif

	projectionMarix = glm::perspective(fovy, aspect, zNear, zFar);
	camera = new Camera();
	camera->translate(glm::vec3(3.0f, 0.5f, 4.0f), VIEW_COORDINATES, VIEW_COORDINATES);
	camera->rotate(45, yAxis, VIEW_COORDINATES);
	camera->push();

	LightSource ls;
	ls.position = glm::vec3(5.0f, 5.0f, 5.0f);
	ls.ambientIntensity = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
	ls.diffuseIntensity = glm::vec4(0.35f, 0.35f, 0.35f, 1.0f);
	ls.specularIntensity = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);

	light = new Light();
	light->setLightSource(&ls);

	model = new Model3D();
	model->loadModel(MODEL_FILE);
	model->scale(0.5, MODEL_COORDINATES);
}

void displayFunc(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Program::useProgram(NULL);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMarix));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(camera->getViewMatrix()));
	drawGroundGrid(0.0f, 0.0f, 50.0f, 50.0f, 0.5f);

	Program::useProgram(program);
	glUniformMatrix4fv(Camera::uViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(uProjMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMarix));

	glm::vec3 viewPos = camera->getViewPoint();
	glUniform3f(Camera::uViewPosLoc, viewPos.x, viewPos.y, viewPos.z);
#ifdef PRINT_CAMERA_POSITION
	cout << "Camera Pos:\t" << viewPos << endl;
#endif

	if (light != NULL){
		glBindBufferRange(GL_UNIFORM_BUFFER, Light::uboLightLoc,
			light->getUBOLight(), 0, sizeof(LightMaterial));
#ifdef PRINT_LIGHT_SOURCE
		cout << *(light->uLight) << endl;
#endif
	}

	model->render();
	glutSwapBuffers();
}

void drawGroundGrid(float centerX, float centerZ, float rangeX, float rangeZ, float step){
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f); //Read Color - Z - axis
	//glVertex3f(0.0f, 0.0f, centerZ - rangeZ);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, centerZ + rangeZ);

	glColor3f(0.0f, 1.0f, 0.0f); //Blue Color - X axis
	//glVertex3f(centerX - rangeX, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(centerX + rangeX, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f); //Black Color
	for (float line = 0; line < rangeX; line += step){
		if (centerX + line != 0){
			glVertex3f(centerX + line, 0.0f, centerZ - rangeZ);
			glVertex3f(centerX + line, 0.0f, centerZ + rangeZ);
		}
		if (centerX - line != 0){
			glVertex3f(centerX - line, 0.0f, centerZ - rangeZ);
			glVertex3f(centerX - line, 0.0f, centerZ + rangeZ);
		}
	}
	for (float line = 0; line < rangeZ; line += step){
		if (centerZ + line != 0){
			glVertex3f(centerX - rangeX, 0.0f, centerZ + line);
			glVertex3f(centerX + rangeX, 0.0f, centerZ + line);
		}
		if (centerZ - line != 0){
			glVertex3f(centerX - rangeX, 0.0f, centerZ - line);
			glVertex3f(centerX + rangeX, 0.0f, centerZ - line);
		}
	}
	glEnd();
}