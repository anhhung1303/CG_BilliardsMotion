#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "mesh.hpp"

class Node{
public:
	static GLuint uMVPMatrixLoc;

	Node();
	virtual ~Node();
	
	Node* parent;
	std::vector<Node*> children;
	glm::mat4 modelMatrix;
	std::vector<Mesh*> content;
};

