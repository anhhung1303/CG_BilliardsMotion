#include "shader.hpp"

Shader::Shader(){
}

Shader::Shader(const std::string& path, GLenum shaderType){
	loadShader(path, shaderType);
}

Shader::~Shader(){
	if (shaderID != NULL){
		glDeleteShader(shaderID);
	}
}

void Shader::loadShader(const std::string& path, GLenum shaderType){
	shaderID = glCreateShader(shaderType);
	if (shaderID == 0){
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		exit(EXIT_FAILURE);
	}

	std::string shaderSource = getSource(path);
	const GLchar *pointer = shaderSource.c_str();
	glShaderSource(shaderID, 1, (const GLchar**)&pointer, NULL);
	glCompileShader(shaderID);

	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE){
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
		exit(EXIT_FAILURE);
	}
}

GLenum Shader::getShaderType() const{
	return this->shaderType;
}

GLuint Shader::getShaderID() const{
	return this->shaderID;
}

Program::Program(){
	this->shaderProgram = glCreateProgram();
	if (this->shaderProgram == 0){
		fprintf(stderr, "Creating shader program ERROR.\n");
		glDeleteProgram(this->shaderProgram);
		exit(EXIT_FAILURE);
	}
}

Program::~Program(){
	glDeleteProgram(this->shaderProgram);
}

void Program::attachShader(Shader* shader){
	GLint status;
	GLchar errorLog[1024] = { 0 };

	glAttachShader(this->shaderProgram, shader->getShaderID());

	glLinkProgram(this->shaderProgram);
	glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) { //Link program false
		glGetProgramInfoLog(this->shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		glDeleteProgram(this->shaderProgram);
		exit(EXIT_FAILURE);
	}

	glValidateProgram(this->shaderProgram);
	glGetProgramiv(this->shaderProgram, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE) { //Invalid program
		glGetProgramInfoLog(this->shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		glDeleteProgram(this->shaderProgram);
		exit(EXIT_FAILURE);
	}
}

void Program::useProgram(bool usage){
	if (usage){
		glUseProgram(this->shaderProgram);
	}
	else {
		glUseProgram(NULL);
	}
}

GLuint Program::getProgram() const{
	return this->shaderProgram;
}