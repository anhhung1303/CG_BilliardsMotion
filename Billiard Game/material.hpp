#pragma once

#include <stdlib.h>

#include <gl/glew.h>
#include <IL/il.h>

#include <iostream>
#include <string>

#define MATERIAL_INVALID 0xFFFFFFFF

/**
* @brief a.k.a Texture
*/
class Material{
public:
	Material();
	Material(const char* path);
	Material(const std::string& path);
	virtual ~Material();
	void clear();

	void loadImage(const char* path);
	void loadImage(const std::string& path);
	void setTexParameter(GLenum parameterName, GLint param);

	GLuint getTextureID();
	GLint getTextureInfo(ILenum mode) const;
	GLint getTextureData() const;
private:
	GLuint textureID;
	GLuint imageID;
};

typedef Material Texture;