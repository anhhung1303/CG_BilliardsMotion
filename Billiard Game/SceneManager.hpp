#pragma once
#include "ResourceManager.hpp"

#include "Scene.hpp"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
public:
	// load the list of scene and then load all the scenes
	void load(const char * listSceneFilePath, ResourceManager * resourceManager);
	// render the scene with id
	void renderScene(int sceneId);
	// delete old data
	void unload();
	Scene * scenes;
private:
	//Scene * scenes;
	int numOfScenes;
};

