#include "material.hpp"

Material::Material(){
	clear();
}

Material::Material(const char* path){
	loadImage(path);
}

Material::Material(const std::string& path){
	loadImage(path.data());
}

Material::~Material(){
	clear();
}

void Material::clear(){
	if (imageID != NULL){
		ilDeleteImage(imageID);
		//ilDeleteImages(1, &imageID);
	}
	if (textureID != NULL){
		glDeleteTextures(1, &textureID);
	}

	imageID = NULL;
	textureID = NULL;
}

void Material::loadImage(const char* path){
	clear();
	ilGenImages(1, &imageID);

	ilBindImage(imageID);
	if (!ilLoadImage(path)){
		clear();
		fprintf(stderr, "Reading texture ERROR!");
		exit(EXIT_FAILURE);
	}
	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)){
		clear();
		fprintf(stderr, "Converting texture ERROR!");
		exit(EXIT_FAILURE);
	}
	
}

void Material::loadImage(const std::string& path){
	loadImage(path.data());
}
GLuint Material::getTextureID(){
	if (textureID != NULL){
		return textureID;
	}

	if (imageID == NULL){
		fprintf(stderr, "NO Texture.");
		exit(EXIT_FAILURE);
	}

	ilBindImage(imageID);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0,
		ilGetInteger(IL_IMAGE_FORMAT),
		ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT),
		0, GL_RGBA, GL_UNSIGNED_BYTE,
		ilGetData());
	glBindTexture(GL_TEXTURE_2D, NULL);

	ilBindImage(NULL);

	return textureID;
}

void Material::setTexParameter(GLenum parameterName, GLint param){
	glBindTexture(GL_TEXTURE_2D, getTextureID());
	glTexParameteri(GL_TEXTURE_2D, parameterName, param);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

GLint Material::getTextureInfo(ILenum mode) const{
	if (imageID == NULL){
		fprintf(stderr, "NO Texture.");
		exit(EXIT_FAILURE);
	}

	ilBindImage(imageID);
	int info = ilGetInteger(mode);
	ilBindImage(NULL);

	return info;
}

GLint Material::getTextureData() const{
	return -1; //Unsupported now
}