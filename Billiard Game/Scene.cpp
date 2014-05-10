#include "Scene.hpp"

Scene::Scene()
{
	objects = NULL;
	numOfObjects = 0;
}


Scene::~Scene()
{
	unload();
}


// load configure information from sceneFile and create objects with resource from resourceManager
void Scene::load(char * sceneFilePath, ResourceManager * resourceManager)
{
	unload();
	FILE * inputFile = fopen(sceneFilePath, "r");

	if (inputFile == NULL){
		cerr << "load file " << sceneFilePath << " failed!!!";
	}

	//Reading object
	fscanf(inputFile, "#Objects: %d\n", &numOfObjects);
	//cout << "Objects = " << numOfObjects << endl;
	objects = new Object * [numOfObjects];
	for (int idObject = 0; idObject < numOfObjects; ++idObject){
		int temp;
		fscanf(inputFile, "ID %d\n", &temp);
		char objectType[300];
		fscanf(inputFile, "OBJECT_TYPE %s\n", objectType);
		cout << "OBJECT_TYPE = " << objectType << endl;
		if (strcmp(objectType, "BALL") == 0){
			objects[idObject] = new Ball();
		}
		else {
			objects[idObject] = new Object();
		}
		//Model
		int idModel;
		fscanf(inputFile, "MODEL %d\n", &idModel);
		//cout << "MODEL = " << idModel << endl;
		objects[idObject]->loadModel(idModel, resourceManager);		
		//Shaders
		int idShaders;
		fscanf(inputFile, "SHADER %d\n", &idShaders);
		//cout << "SHADER = " << idShaders << endl;
		objects[idObject]->loadProgram(idShaders, resourceManager);
		//Object position, rotation, scale
		float x, y, z;
		fscanf(inputFile, "POSITION %f, %f, %f\n", &x, &y, &z);
		objects[idObject]->translate(x, y, z);
		fscanf(inputFile, "ROTATION %f, %f, %f\n", &x, &y, &z);
		//TODO
		float scaleVal;
		fscanf(inputFile, "SCALE %f\n", &scaleVal);
		objects[idObject]->scale(scaleVal);
	}

	//LIGHTS
	fscanf(inputFile, "#LIGHTS\n");
	float x, y, z;
	fscanf(inputFile, "AmbientColor %f, %f, %f\n", &x, &y, &z);
	fscanf(inputFile, "AmbientWeight %f\n", &x);
	int numOfLights;
	fscanf(inputFile, "LightsCount %d\n", &numOfLights);
	for (int idLight = 0; idLight < numOfLights; ++idLight){
		int temp;
		fscanf(inputFile, "ID %d\n", &temp);
		fscanf(inputFile, "POS/DIR %f, %f, %f\n", &x, &y, &z);
		char type[10];
		fscanf(inputFile, "TYPE %s\n", type);
		fscanf(inputFile, "COLOR %f, %f, %f\n", &x, &y, &z);
		char moving[10];
		fscanf(inputFile, "MOVING %s\n", moving);
		float radius;
		fscanf(inputFile, "RADIUS %f\n", &radius);
		float speed;
		fscanf(inputFile, "SPEED %f\n", &speed);
	}

	//CAMERA
	fscanf(inputFile, "#CAMERA\n");

	fscanf(inputFile, "POSITION %f, %f, %f\n", &x, &y, &z);
	camera.push();
	camera.translate(glm::vec3(x, y, z));
	//cout << "Camera position = " << x << " " << y << " " << z << endl;

	GLfloat zNear, zFar, fovy, aspect;
	fscanf(inputFile, "NEAR %f\n", &zNear);
	fscanf(inputFile, "FAR %f\n", &zFar);
	fscanf(inputFile, "FOV %f\n", &fovy);
	fscanf(inputFile, "ASPECT %f\n", &aspect);
	projectionMarix = glm::perspective(fovy, aspect, zNear, zFar);
	//cout << fovy << " " << aspect  << " " << zNear << " " << zFar << endl;

	fclose(inputFile);
}

// render 3D scene
void Scene::render()
{
	for (int objectId = 0; objectId < numOfObjects; ++objectId){
		objects[objectId]->render(projectionMarix, &camera);
	}
}

// delete old data
void Scene::unload()
{
	if (objects != NULL){
		for (int objectId = 0; objectId < numOfObjects; ++objectId){
			objects[objectId]->unload();
			if (objects[objectId] != NULL){
				delete objects[objectId];
				objects[objectId] = NULL;
			}
		}
		delete[] objects;
		objects = NULL;
		numOfObjects = 0;
	}
}
