#include "node.hpp"

GLuint Node::uMVPMatrixLoc = 1;

Node::Node(){
	parent = NULL;
	children.clear();
	content.clear();
	modelMatrix = glm::mat4(1.0f);
}

Node::~Node(){
}
