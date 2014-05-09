#pragma once

#include <string>

#include <gl/glew.h>

#include "util.hpp"

class Shader{
public:
	Shader();
	Shader(const std::string& path, GLenum shaderType);
	virtual ~Shader();

	void loadShader(const std::string& path, GLenum shaderType);

	GLenum getShaderType() const;
	GLuint getShaderID() const;
private:
	GLenum shaderType;
	GLuint shaderID;
};

class Program{
public:
	Program();
	virtual ~Program();

	void attachShader(Shader* shader);
	void useProgram(bool usage);

	GLuint getProgram() const;
private:
	GLuint shaderProgram;
};