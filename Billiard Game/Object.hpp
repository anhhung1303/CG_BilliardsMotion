#pragma once
#include "ResourceManager.hpp"
#include "model3D.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "Constant.hpp"

class Object
{
public:
	Object();
	~Object();
	// render the 3D object
	virtual void render(Camera * camera, Light * light, GLdouble elapsedTime);
	// load Model3D from resourceManager
	void loadModel(int modelId, ResourceManager * resourceManager);
	// load Program from resourceManager
	void loadProgram(int programId, ResourceManager * resourceManager);
	void unload();

	void translate(float x, float y, float z);
	void translate(glm::vec3 translateVector);
	void rotate(float angle, float x, float y, float z);
	void rotate(float angle, glm::vec3 rotateVector);
	void scale(float x);

	glm::mat4 getModelMatrix() const;
private:
	Model3D * model;
	Program * program;

	glm::mat4 modelMatrix;
};

