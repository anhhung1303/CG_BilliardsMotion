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

#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <IL/il.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>

#include "shader.hpp"
#include "Camera.hpp"
#include "vertex.hpp"

using namespace std;

GLuint VBO;
GLuint IBO;
GLuint textureID;
GLuint gMVP;

char* vertexShaderPath = "./Resource/Shader/shader.vert";
char* fragmentShaderPath = "./Resource/Shader/shader.frag";
char* texturePath = "./Resource/Model/TextureDemo.png";
Program * shader;
Camera * camera;

void showMatrix(glm::mat4 matrix){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			cout << matrix[i][j] << "\t";
		}
		cout << endl;
	}
}

static void displayFunc(){
	glClear(GL_COLOR_BUFFER_BIT);

	static float scale = 0.0f;

	scale += 0.001f;

	//static glm::mat4 modelMatrix;
	//modelMatrix = glm::mat4(1.0f);
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(sinf(scale), 0.0f, 0.0f));
	//modelMatrix = glm::rotate(modelMatrix, sinf(scale)* 90.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(sinf(scale * 0.1f), sinf(scale * 0.1f), sinf(scale * 0.1f)));
	//glUniformMatrix4fv(gMVP, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 MVPMatrix = camera->getViewMatrix();
	showMatrix(MVPMatrix);
	glUniformMatrix4fv(gMVP, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glutSwapBuffers();
}

static void keyboardFunc(unsigned char key, int x, int y){
	switch (key){
	case 'J': case 'j':
		camera->zoom(0.8f);
		break;
	case 'L': case 'l':
		camera->zoom(1.2f);
		break;
	case 'A': case 'a':
		camera->rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case 'D': case 'd':
		camera->rotate(-1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case 'W': case 'w':
		camera->rotate(1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case 'S': case 's':
		camera->rotate(-1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case 'Q': case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

static void keyboardSpecial(int key, int x, int y){
	switch (key) {
	case GLUT_KEY_UP:
		camera->translate(glm::vec3(0.0f, 0.5f, 0.0f));
		break;
	case GLUT_KEY_DOWN:
		camera->translate(glm::vec3(0.0f, -0.5f, 0.0f));
		break;
	case GLUT_KEY_LEFT:
		camera->translate(glm::vec3(0.5f, 0.0f, 0.0f));
		break;
	case GLUT_KEY_RIGHT:
		camera->translate(glm::vec3(-0.5f, 0.0f, 0.0f));
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

glm::vec3 computeNormal(glm::vec3 const & a,
	glm::vec3 const & b,
	glm::vec3 const & c){
	return glm::normalize(glm::cross(c - a, b - a));
}

static void createBuffer(){
	//Vertex vertices[4] = {
	//	Vertex(glm::fvec3(-1.0f, -1.0f, 0.5773f), glm::vec2(0.0f, 0.0f)),
	//	Vertex(glm::fvec3(0.0f, -1.0f, -1.15475f), glm::vec2(0.5f, 0.0f)),
	//	Vertex(glm::fvec3(1.0f, -1.0f, 0.5773f), glm::vec2(1.0f, 0.0f)),
	//	Vertex(glm::fvec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 1.0f)),
	//};

	Vertex vertices[4];
	vertices[0].pos = glm::fvec3(-1.0f, -1.0f, 0.5773f);
	vertices[1].pos = glm::fvec3(0.0f, -1.0f, -1.15475f);
	vertices[2].pos = glm::fvec3(1.0f, -1.0f, 0.5773f);
	vertices[3].pos = glm::fvec3(0.0f, 1.0f, 0.0f);

	vertices[0].tex = glm::vec2(0.0f, 0.0f);
	vertices[1].tex = glm::vec2(0.5f, 0.0f);
	vertices[2].tex = glm::vec2(1.0f, 0.0f);
	vertices[3].tex = glm::vec2(0.5f, 1.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	for (int i = 0; i < 12; i += 3){
		glm::vec3 normal = computeNormal(vertices[indices[i + 0]].pos,
			vertices[indices[i + 1]].pos,
			vertices[indices[i + 2]].pos);
		vertices[indices[i + 0]].nor += normal;
		vertices[indices[i + 1]].nor += normal;
		vertices[indices[i + 2]].nor += normal;
	}

	for (int i = 0; i < 4; i++){
		vertices[i].nor = glm::normalize(vertices[i].nor);
	}
}

static void readTexture(){
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	unsigned int imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	if (!ilLoadImage(texturePath)){
		ilDeleteImages(1, &imageID);
		fprintf(stderr, "Error while read texture");
		exit(-1);
	}

	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)){
		ilDeleteImages(1, &imageID);
		fprintf(stderr, "Error while convert texture");
		exit(-1);
	}

	cout << "Width :" << ilGetInteger(IL_IMAGE_WIDTH) << endl
		<< "Height: " << ilGetInteger(IL_IMAGE_HEIGHT) << endl
		<< "Format: " << ilGetInteger(IL_IMAGE_FORMAT) << endl;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0,
		ilGetInteger(IL_IMAGE_FORMAT),
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		0, GL_RGBA, GL_UNSIGNED_BYTE,
		ilGetData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//static void createBuffer(){
//	glm::vec3 Vertices[4] = {
//		glm::fvec3(-1.0f, -1.0f, 0.5773f),
//		glm::fvec3(0.0f, -1.0f, -1.15475f),
//		glm::fvec3(1.0f, -1.0f, 0.5773f),
//		glm::fvec3(0.0f, 1.0f, 0.0f),
//	};
//
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
//
//	unsigned int indices[] = {
//		0, 3, 1,
//		1, 3, 2,
//		2, 3, 0,
//		0, 1, 2 };
//
//	glGenBuffers(1, &IBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//}

static void initData(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	shader = new Program();
	shader->attachShader(new Shader(vertexShaderPath, GL_VERTEX_SHADER));
	shader->attachShader(new Shader(fragmentShaderPath, GL_FRAGMENT_SHADER));
	GLuint p = shader->getProgram();
	shader->useProgram(true);

	camera = new Camera();

	createBuffer();
	readTexture();
	gMVP = glGetUniformLocation(p, "gMVP");
	//gMVP = shader->getUniform("gMVP");
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 13. Camera Space");

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	printf("GL version: %s\n", glGetString(GL_VERSION));

	initData();

	glutDisplayFunc(displayFunc);
	//glutIdleFunc(displayFunc);
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(keyboardSpecial);
	glutMainLoop();

	return 0;
}