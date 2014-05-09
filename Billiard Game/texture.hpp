#pragma once

#include <string>

#include <gl/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include "util.hpp"

class Texture{
private:
	GLuint texUnit;
public:
	static GLuint uTextureCountLoc;

	Texture();
	virtual ~Texture();
	void clear();

	bool loadTexture(const std::string& path);
	GLuint getTextureUnit() const;
};

