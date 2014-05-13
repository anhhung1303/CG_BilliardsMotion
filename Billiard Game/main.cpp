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
#include "shader.hpp"
#include "model3D.hpp"
#include "light.hpp"
#include "util.hpp"

using namespace std;

#define VERTEX_SHADER_FILE		"./Resource/Shader/basic_light.vert"
#define FRAGMENT_SHADER_FILE	"./Resource/Shader/basic_light.frag"
#define MODEL_FILE				"./Resource/Model/box.3ds"
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
void specialKeyFunc(int key, int x, int y);
void keyboardFunc(unsigned char key, int x, int y);
void displayFunc();
void drawGroundGrid(float centerX, float centerZ, float rangeX, float rangeZ, float step);

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
	printf("===========================================================\n");

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
	program->printActiveUniform();

	projectionMarix = glm::perspective(fovy, aspect, zNear, zFar);
	camera = new Camera();
	camera->translate(glm::vec3(0.0f, 0.0f, -0.34f));
	camera->push();

	LightSource ls;
	ls.position = glm::vec3(5.0f, 5.0f, 5.0f);
	ls.ambientIntensity = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
	//ls.diffuseIntensity = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
	ls.specularIntensity = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);

	light = new Light();
	light->setLightSource(&ls);

	model = new Model3D();
	model->loadModel(MODEL_FILE);
	model->scale(0.01f);
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

	Program::useProgram(NULL);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMarix));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(camera->getViewMatrix()));
	drawGroundGrid(0.0f, 0.0f, 50.0f, 50.0f, 0.1f);

	Program::useProgram(program);
	glUniformMatrix4fv(Camera::uViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glUniformMatrix4fv(uProjMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMarix));

	glm::vec3 viewPos = camera->getViewPosition();
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