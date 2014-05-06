#pragma once

#include <glm/glm.hpp>

class Vertex{
public:
#define VERTEX_POSITION				1
#define VERTEX_TEXTURE_COORDINATES	2
#define VERTEX_NORMAL				3

	Vertex(const glm::vec3 &pos = glm::vec3(0.0f),
		const glm::vec2 &tex = glm::vec2(0.0f),
		const glm::vec3 &nor = glm::vec3(0.0f));
	virtual ~Vertex();

	void setPosition(const glm::vec3& position);
	void setTexCoor(const glm::vec2& texCoor);
	void setNormal(const glm::vec3& normal);

	const glm::vec3& getPosition() const;
	const glm::vec2& getTexCoor() const;
	const glm::vec3& getNormal() const;
	static const void* getOffset(int type);
private:
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 nor;
};

