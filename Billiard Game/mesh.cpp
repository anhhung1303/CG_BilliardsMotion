#include "mesh.hpp"

Mesh::Mesh(){
	clear();
}

Mesh::Mesh(const std::vector<Vertex>& vertices,
	const std::vector<unsigned int>& indices){
	setVertices(vertices);
	setIndices(indices);
}

Mesh::~Mesh(){
	clear();
}

void Mesh::clear(){
	if (vertexBufferID != NULL){
		glDeleteBuffers(1, &vertexBufferID);
	}
	if (indexBufferID != NULL){
		glDeleteBuffers(1, &indexBufferID);
	}

	vertexBufferID = NULL;
	indexBufferID = NULL;
	numIndices = 0;
	numVertices = 0;
	material = NULL;
}

void Mesh::setVertices(const std::vector<Vertex>& vertices){
	this->numVertices = vertices.size();

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* numVertices, (GLvoid*) vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Mesh::setIndices(const std::vector<GLuint>& indices){
	this->numIndices = indices.size();

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndices, (GLvoid*) indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void Mesh::setMaterial(Material* material){
	this->material = material;
}

GLuint Mesh::getVertexBufferID() const{
	return this->vertexBufferID;
}

GLuint Mesh::getIndexBufferID() const{
	return this->indexBufferID;
}

GLuint Mesh::getNumVertices() const{
	return this->numVertices;
}

GLuint Mesh::getNumIndices() const{
	return this->numIndices;
}

Material* Mesh::getMaterial() const{
	return this->material;
}

GLuint Mesh::getVertexBufferData() const{
	return -1; //Unsupported now
}

GLuint Mesh::getIndexBufferData() const{
	return -1; //Unsupported now
}