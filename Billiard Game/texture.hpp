#pragma once

#include <string>

#include <gl/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "util.hpp"

class Texture{
public:
	static GLuint uTextureUnitLoc;

	Texture();
	virtual ~Texture();
	void clear();

	bool loadTexture(const std::string& path);
	GLuint getTextureUnit() const;
private:
	GLuint texUnit;
};

