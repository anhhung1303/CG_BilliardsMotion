#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>

GLuint vbo;
GLuint uboMaterial;

void createTest(){
	static glm::vec3 position[] = {
		glm::vec3(-5, 5, 0),
		glm::vec3(-5, -5, 0),
		glm::vec3(5, -5, 0),
		glm::vec3(5, 5, 0),
	};

	//static glm::vec3 normal
}