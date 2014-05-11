#pragma once

#include "ResourceManager.hpp"
#include "Object.hpp"
#include "Ball.hpp"

class Scene
{
public:
	Scene();
	~Scene();
	// load configure information from sceneFile and create objects with resource from resourceManager
	void load(char * sceneFilePath, ResourceManager * resourceManager);
	// render 3D scene
	void render();
	// delete old data
	void unload();

	Camera * getUsingCamera();
	void setUsingCamera(int cameraId);
	
	Object * * objects;//TODO set private
private:
	Camera * cameras;
	int numOfCameras;
	int usingCameraId;

	//Object * * objects;
	int numOfObjects;

	glm::mat4 projectionMarix;
};

