#include "vertex.hpp"

Vertex::Vertex(){
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

void Vertex::setTangent(const glm::vec3& tangent){
	this->tag = tangent;
}

void Vertex::setBitangent(const glm::vec3& bitangent){
	this->bit = bitangent;
}

void Vertex::setColor(const glm::vec4& color){
	this->col = color;
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

const glm::vec3& Vertex::getTangent() const{
	return this->tag;
}

const glm::vec3& Vertex::getBitangent() const{
	return this->bit;
}

const glm::vec4& Vertex::getColor() const{
	return this->col;
}

const void* Vertex::getOffset(int type){
	int offset = 0;
	switch (type){
	case VERTEX_COLOR:
		offset += sizeof(glm::vec3); //type of bit
	case VERTEX_BITANGENT:
		offset += sizeof(glm::vec3); //type of tag
	case VERTEX_TANGENT:
		offset += sizeof(glm::vec3); //type of nor
	case VERTEX_NORMAL:
		offset += sizeof(glm::vec2); //type of tex
	case VERTEX_TEXTURE_COORDINATES:
		offset += sizeof(glm::vec3); //type of pos
	case VERTEX_POSITION:
		return (void*) offset;
	default:
		return NULL; //Invalid type
		break;
	}
}