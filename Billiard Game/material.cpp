#include "material.hpp"

GLuint Material::uboMaterialLoc = 1;

Material::Material(){
	clear();
}

Material::~Material(){
	clear();
}

void Material::clear(){
	glDeleteBuffers(1, &uboMaterial);
}

bool Material::setMaterial(const aiMaterial* material){
	aiColor4D color;

	uMaterial.ambient = (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS) ?
		glm::vec4(color.r, color.g, color.b, color.a) :
		glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

	uMaterial.diffuse = (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) ?
		glm::vec4(color.r, color.g, color.b, color.a) :
		glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

	uMaterial.specular = (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) ?
		glm::vec4(color.r, color.g, color.b, color.a) :
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	uMaterial.emissive = (material->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) ?
		glm::vec4(color.r, color.g, color.b, color.a) :
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	uMaterial.shininess = 0.0;
	unsigned int max;
	aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &uMaterial.shininess, &max);

	glGenBuffers(1, &uboMaterial);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMaterial);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialBlock), (void*)(&uMaterial), GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, uboMaterialLoc, uboMaterial, 0, sizeof(MaterialBlock));
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);

	return true;
}

GLuint Material::getUBOMaterial() const{
	return this->uboMaterial;
}