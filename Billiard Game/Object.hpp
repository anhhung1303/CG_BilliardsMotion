#pragma once
#include "ResourceManager.hpp"
#include "model3D.hpp"
#include "shader.hpp"
#include "camera.hpp"

class Object
{
public:
	Object();
	~Object();
	// render the 3D object
	void render(glm::mat4 projectionMarix, Camera * camera);
	// load Model3D from resourceManager
	void loadModel(int modelId, ResourceManager * resourceManager);
	// load Program from resourceManager
	void loadProgram(int programId, ResourceManager * resourceManager);
	void unload();

	void translate(float x, float y, float z);
	void rotate();//TODO
	void scale(float x);
private:
	Model3D * model;
	Program * program;
};

