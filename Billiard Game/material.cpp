#include "material.hpp"

GLuint Material::uboMaterialLoc = 1;

LightMaterial::LightMaterial(){
	ambient = glm::vec4(0.2, 0.2, 0.2, 1.0);
	diffuse = glm::vec4(0.8, 0.8, 0.8, 1.0);
	specular = glm::vec4(0.0, 0.0, 0.0, 1.0);
	shininess = 0.0f;
	emissive = glm::vec4(0.0, 0.0, 0.0, 1.0);
}

Material::Material(){
	//clear();
	uMaterial = new LightMaterial();
}

Material::~Material(){
	clear();
}

void Material::clear(){
	glDeleteBuffers(1, &uboMaterial);
	if (uMaterial != NULL){
		delete uMaterial;
		uMaterial = NULL;
	}
}

bool Material::setMaterial(const aiMaterial* material){
	clear();
	this->uMaterial = new LightMaterial();
	aiColor4D color;

	if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS){
		uMaterial->ambient = glm::vec4(color.r, color.g, color.b, color.a);
	}

	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS){
		uMaterial->diffuse = glm::vec4(color.r, color.g, color.b, color.a);
	}

	if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS){
		uMaterial->specular = glm::vec4(color.r, color.g, color.b, color.a);
	}

	if (material->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS){
		uMaterial->emissive = glm::vec4(color.r, color.g, color.b, color.a);
	}

	uMaterial->shininess = 0.0;
	unsigned int max;
	aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &(uMaterial->shininess), &max);

	glGenBuffers(1, &uboMaterial);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMaterial);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightMaterial), (void*) uMaterial, GL_STATIC_DRAW);
	//glBindBufferRange(GL_UNIFORM_BUFFER, uboMaterialLoc, uboMaterial, 0, sizeof(LightMaterial));
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);

	return true;
}

GLuint Material::getUBOMaterial() const{
	return this->uboMaterial;
}