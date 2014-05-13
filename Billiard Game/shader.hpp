#pragma once

#include <gl/glew.h>

#include <string>
#include "util.hpp"

class Shader{
private:
	GLenum shaderType;
	GLuint shaderID;
public:
	Shader();
	Shader(const std::string& path, GLenum shaderType);
	virtual ~Shader();

	void loadShader(const std::string& path, GLenum shaderType);

	GLenum getShaderType() const;
	GLuint getShaderID() const;
};

class Program{
private:
	GLuint shaderProgram;
public:
	Program();
	virtual ~Program();

	void attachShader(Shader* shader);
	static void useProgram(const Program* prog);

	GLuint getProgram() const;
	void printActiveUniform() const;
};