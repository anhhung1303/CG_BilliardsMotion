#include <gl/glew.h>
#include <glm/glm.hpp>
#include "mesh.hpp"

glm::vec3 computeNormal(glm::vec3 const & a,
	glm::vec3 const & b,
	glm::vec3 const & c){
	return glm::normalize(glm::cross(c - a, b - a));
}

GLuint createVBO(){
	GLuint VBO;
	glGenVertexArrays(1, &VBO);
	glBindVertexArray(VBO);

	GLuint buffer;
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glm::vec3 pos[4];
	pos[0] = glm::fvec3(-1.0f, -1.0f, 0.5773f);
	pos[1] = glm::fvec3(0.0f, -1.0f, -1.15475f);
	pos[2] = glm::fvec3(1.0f, -1.0f, 0.5773f);
	pos[3] = glm::fvec3(0.0f, 1.0f, 0.0f);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Mesh::aPositionLoc); //0 is mean position
	glVertexAttribPointer(Mesh::aPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glm::vec2 tex[4];
	tex[0] = glm::vec2(0.0f, 0.0f);
	tex[1] = glm::vec2(1.0f, 0.0f);
	tex[2] = glm::vec2(0.5f, 1.0f);
	tex[3] = glm::vec2(0.5f, 5.0f);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Mesh::aTexCoordLoc); //1 is mean texture coordinate
	glVertexAttribPointer(Mesh::aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glm::vec3 nor[4];
	for (int i = 0; i < 12; i += 3){
		glm::vec3 normal = computeNormal(pos[indices[i + 0]], pos[indices[i + 1]], pos[indices[i + 2]]);
		nor[indices[i + 0]] += normal;
		nor[indices[i + 1]] += normal;
		nor[indices[i + 2]] += normal;
	}
	for (int i = 0; i < 4; i++){
		nor[i] = glm::normalize(nor[i]);
	}
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(nor), nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Mesh::aNormalLoc); //2 is mean normal
	glVertexAttribPointer(Mesh::aNormalLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

	return VBO;
}