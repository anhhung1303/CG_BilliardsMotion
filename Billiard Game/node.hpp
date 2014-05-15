#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "mesh.hpp"

class Node{
public:
//private:
	Node* parent;
	std::vector<Node*> children;
	glm::mat4 modelMatrix;
	std::vector<Mesh*> content;
public:
	static GLuint uModelMatrixLoc;

	Node();
	virtual ~Node();
};
