#include "SceneManager.hpp"

#include "ResourceManager.hpp"


SceneManager::SceneManager()
{
	scenes = NULL;
}


SceneManager::~SceneManager()
{
	unload();
}


// load the list of scene and then load all the scenes
void SceneManager::load(const char * listSceneFilePath, ResourceManager * resourceManager)
{
	unload();
	FILE * inputFile = fopen(listSceneFilePath, "r");

	if (inputFile == NULL){
		cerr << "load file " << listSceneFilePath << " failed!!!";
	}

	//Reading object
	fscanf(inputFile, "#Scenes: %d\n", &numOfScenes);
	scenes = new Scene[numOfScenes];
	for (int idScene = 0; idScene < numOfScenes; ++idScene){
		int temp;
		fscanf(inputFile, "ID %d\n", &temp);
		char fileScene[300];
		fscanf(inputFile, "SCENE \"%[^\"]\"\n", fileScene);
		//cout << fileScene;
		scenes[idScene].load(fileScene, resourceManager);
	}
	fclose(inputFile);
}


// render the scene with id
void SceneManager::renderScene(int sceneId)
{
	scenes[sceneId].render();
}


// delete old data
void SceneManager::unload()
{
	if (scenes != NULL){
		delete[] scenes;
		scenes = NULL;
	}
}

Scene * SceneManager::getScene(int sceneId){
	return &(scenes[sceneId]);
}
