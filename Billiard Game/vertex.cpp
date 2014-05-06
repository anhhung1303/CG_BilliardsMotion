#include "vertex.hpp"

Vertex::Vertex(const glm::vec3 &pos,
	const glm::vec2 &tex,
	const glm::vec3 &nor){
	setPosition(pos);
	setTexCoor(tex);
	setNormal(nor);
}

Vertex::~Vertex(){
}

void Vertex::setPosition(const glm::vec3& position){
	this->pos = position;
}

void Vertex::setTexCoor(const glm::vec2& texCoor){
	this->tex = texCoor;
}

void Vertex::setNormal(const glm::vec3& normal){
	this->nor = normal;
}

const glm::vec3& Vertex::getPosition() const{
	return this->pos;
}

const glm::vec2& Vertex::getTexCoor() const{
	return this->tex;
}

const glm::vec3& Vertex::getNormal() const{
	return this->nor;
}

const void* Vertex::getOffset(int type){
	int offset = 0;
	switch (type){
	case VERTEX_NORMAL:
		offset += sizeof(glm::vec2); //typeof tex
	case VERTEX_TEXTURE_COORDINATES:
		offset += sizeof(glm::vec3); //typeof pos
	case VERTEX_POSITION:
		return (void*) offset;
	default:
		return NULL; //Invalid type
		break;
	}
}