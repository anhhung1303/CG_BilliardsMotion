#pragma once

#include <gl/glew.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

struct MaterialBlock{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emissive;
	float shininess;
};

class Material{
private:
	MaterialBlock uMaterial;

	//Uniform Buffer Object of Material
	GLuint uboMaterial;
public:
	static GLuint uboMaterialLoc;

	Material();
	virtual ~Material();

	void clear();
	bool setMaterial(const aiMaterial* material);
	GLuint getUBOMaterial() const;
};