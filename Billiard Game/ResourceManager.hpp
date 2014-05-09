#pragma once
#include "model3D.hpp"
#include "shader.hpp"

#include <iostream>

using namespace std;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	// function to load resources before drawing
	void load(const char * resourceFilePath);
	// function to unload (clear memory spent for) resources
	void unload();
	// get the id model from list models
	Model3D * getModel(int modelId);
	// get the id shader program from list programs
	Program * getProgram(int programId);
private:
	Model3D * models;
	Program * programs;

	int numOfModels;
	int numOfPrograms;
};

