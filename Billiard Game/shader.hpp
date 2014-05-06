#pragma once

#include <stdlib.h>

#include <gl/glew.h>
#include <gl/glut.h>

#include <fstream>
#include <string>

class Shader{
public:
	Shader(const char* path, GLenum shaderType);
	Shader(const std::string& path, GLenum shaderType);
	virtual ~Shader();

	void loadShader(const char* path, GLenum shaderType);
	void loadShader(const std::string& path, GLenum shaderType);

	GLenum getShaderType() const;
	GLuint getShaderID() const;
private:
	GLenum shaderType;
	GLuint shaderID;

	std::string getShaderSource(const char* path) const;
};

class Program{
public:
	Program();
	virtual ~Program();

	void attachShader(Shader* shader);
	void useProgram(bool usage);

	GLuint getUniformLocation(std::string uniform);
	GLuint getAttributeLocation(std::string attribute);
private:
	GLuint shaderProgram;
};