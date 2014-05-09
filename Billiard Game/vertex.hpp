#pragma once

#include <glm/glm.hpp>

#ifndef GLM_SWIZZLE
#define GLM_SWIZZLE
#endif

#define VERTEX_POSITION				1
#define VERTEX_TEXTURE_COORDINATES	2
#define VERTEX_NORMAL				3
#define VERTEX_TANGENT				4
#define VERTEX_BITANGENT			5
#define VERTEX_COLOR				6

class Vertex{
private:
	glm::vec3 pos;	//Position (coordinate) of vertex
	glm::vec2 tex;	//Texture coordinate
	glm::vec3 nor;	//Normal vector
	glm::vec3 tag;	//Tangent vector
	glm::vec3 bit;	//Bitangent vector
	glm::vec4 col;	//Color vector (RGBA)
public:
	Vertex();
	~Vertex();

	void setPosition(const glm::vec3& position);
	void setTexCoor(const glm::vec2& texCoor);
	void setNormal(const glm::vec3& normal);
	void setTangent(const glm::vec3& tangent);
	void setBitangent(const glm::vec3& bitangent);
	void setColor(const glm::vec4& color);

	const glm::vec3& getPosition() const;
	const glm::vec2& getTexCoor() const;
	const glm::vec3& getNormal() const;
	const glm::vec3& getTangent() const;
	const glm::vec3& getBitangent() const;
	const glm::vec4& getColor() const;

	static const void* getOffset(int type);
};

