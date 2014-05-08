#pragma once

#include <gl/glew.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

struct UniformMaterialBlock{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emissive;
	float shininess;
};

class Material{
public:
	static GLuint uMaterialBlockLoc;

	Material();
	virtual ~Material();

	void clear();
	bool setMaterial(const aiMaterial* material);
	GLuint getMaterialBlockBuffer() const;
private:
	UniformMaterialBlock uMaterial;

	GLuint uMaterialBlockBuffer;
};