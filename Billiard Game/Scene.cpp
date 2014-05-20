#include "Scene.hpp"

Scene::Scene()
{
	objects = NULL;
	numOfObjects = 0;

	cameras = NULL;
	numOfCameras = 0;

	lights = NULL;
	numOfLights = 0;

	timeLastFrame = 0.0f;
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
		float scaleVal;
		fscanf(inputFile, "SCALE %f\n", &scaleVal);
		objects[idObject]->scale(scaleVal);
		float x, y, z;
		fscanf(inputFile, "POSITION %f, %f, %f\n", &x, &y, &z);
		objects[idObject]->translate(x, y, z);
		fscanf(inputFile, "ROTATION %f, %f, %f\n", &x, &y, &z);
		//TODO

	}

	//LIGHTS
	fscanf(inputFile, "#LIGHTS\n");
	float x, y, z;
	fscanf(inputFile, "AmbientColor %f, %f, %f\n", &x, &y, &z);
	fscanf(inputFile, "AmbientWeight %f\n", &x);
	int numOfLights;
	fscanf(inputFile, "LightsCount %d\n", &numOfLights);
	lights = new Light[numOfLights];
	for (int idLight = 0; idLight < numOfLights; ++idLight){
		int temp;
		fscanf(inputFile, "ID %d\n", &temp);
		fscanf(inputFile, "POS/DIR %f, %f, %f\n", &x, &y, &z);
		LightSource * lightSource = new LightSource();
		lightSource->position = glm::vec3(x, y, z);
		lightSource->ambientIntensity = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
		lightSource->diffuseIntensity = glm::vec4(0.35f, 0.35f, 0.35f, 1.0f);
		lightSource->specularIntensity = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
		lights[idLight].setLightSource(lightSource);
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
	fscanf(inputFile, "#Camera: %d\n", &numOfCameras);
	cameras = new Camera[numOfCameras];
	for (int idCamera = 0; idCamera < numOfCameras; ++idCamera){
		int temp;
		fscanf(inputFile, "ID %d\n", &temp);
		fscanf(inputFile, "POSITION %f, %f, %f\n", &x, &y, &z);		
		cameras[idCamera].translate(glm::vec3(x, y, z), WORLD_COORDINATES, WORLD_COORDINATES);
		float angle;
		fscanf(inputFile, "ROTATION %f, %f, %f, %f\n", &angle, &x, &y, &z);
		cameras[idCamera].rotate(angle, glm::vec3(x, y, z), WORLD_COORDINATES);
		//cout << "Camera position = " << x << " " << y << " " << z << endl;

		GLfloat zNear, zFar, fovy, aspect;
		fscanf(inputFile, "NEAR %f\n", &zNear);
		fscanf(inputFile, "FAR %f\n", &zFar);
		fscanf(inputFile, "FOV %f\n", &fovy);
		fscanf(inputFile, "ASPECT %f\n", &aspect);
		cameras[idCamera].setProjectionMatrix(glm::perspective(fovy, aspect, zNear, zFar));
		//cout << fovy << " " << aspect  << " " << zNear << " " << zFar << endl;
		cameras[idCamera].push();
	}
	
	setUsingCamera(0);
	fclose(inputFile);
}

// render 3D scene
void Scene::render()
{
	GLdouble elapsedTime = glutGet(GLUT_ELAPSED_TIME) - timeLastFrame;

	Program::useProgram(NULL);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(getUsingCamera()->getProjectionMatrix()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(getUsingCamera()->getViewMatrix()));
	drawGroundGrid(0.0f, 0.0f, 50.0f, 50.0f, 0.1f);

	for (int objectId = 0; objectId < numOfObjects; ++objectId){
		objects[objectId]->render(getUsingCamera(), &lights[0], elapsedTime);
		if (elapsedTime > Constant::TIME_FOR_A_FRAME){
			if (objectId > 0){
				Ball * ball = (Ball *)(this->objects[objectId]);
				ball->collideWithTable();
				for (int otherObjectId = objectId + 1; otherObjectId < numOfObjects; ++otherObjectId){
					//cout << "Collision test: " << objectId << " collides with " << otherObjectId << endl;
					Ball * otherBall = (Ball *)(this->objects[otherObjectId]);
					ball->collideWithOtherBall(otherBall);
				}
			}
			timeLastFrame = glutGet(GLUT_ELAPSED_TIME);
		}
	}

	if (elapsedTime > Constant::TIME_FOR_A_FRAME){
		timeLastFrame = glutGet(GLUT_ELAPSED_TIME);
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
	if (cameras != NULL){
		delete[] cameras;
		cameras = NULL;
		numOfCameras = 0;
	}
	if (lights != NULL){
		delete[] lights;
		lights = NULL;
		numOfLights = 0;
	}
}

Camera * Scene::getUsingCamera(){
	if (cameras == NULL || usingCameraId >= numOfCameras){
		cerr << "Get unknown camera " << endl;
		return NULL;
	}
	return &(cameras[usingCameraId]);
}

void Scene::setUsingCamera(int usingCameraId){
	if (usingCameraId < numOfCameras && usingCameraId >= 0){
		this->usingCameraId = usingCameraId;
	}
	else {
		cerr << "set using unknown camera " << endl;
	}
}

void Scene::drawGroundGrid(float centerX, float centerZ, float rangeX, float rangeZ, float step){
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f); //Read Color - Z - axis
	//glVertex3f(0.0f, 0.0f, centerZ - rangeZ);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, centerZ + rangeZ);

	glColor3f(0.0f, 1.0f, 0.0f); //Blue Color - X axis
	//glVertex3f(centerX - rangeX, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(centerX + rangeX, 0.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f); //Black Color
	for (float line = 0; line < rangeX; line += step){
		if (centerX + line != 0){
			glVertex3f(centerX + line, 0.0f, centerZ - rangeZ);
			glVertex3f(centerX + line, 0.0f, centerZ + rangeZ);
		}
		if (centerX - line != 0){
			glVertex3f(centerX - line, 0.0f, centerZ - rangeZ);
			glVertex3f(centerX - line, 0.0f, centerZ + rangeZ);
		}
	}
	for (float line = 0; line < rangeZ; line += step){
		if (centerZ + line != 0){
			glVertex3f(centerX - rangeX, 0.0f, centerZ + line);
			glVertex3f(centerX + rangeX, 0.0f, centerZ + line);
		}
		if (centerZ - line != 0){
			glVertex3f(centerX - rangeX, 0.0f, centerZ - line);
			glVertex3f(centerX + rangeX, 0.0f, centerZ - line);
		}
	}
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-50.0f, 0.0f, 0.07f);
	glVertex3f(50.0f, 0.0f, 0.07f);
	glVertex3f(-50.0f, 0.0f, -0.07f);
	glVertex3f(50.0f, 0.0f, -0.07f);

	glVertex3f(0.143f, 0.0f, -50.0f);
	glVertex3f(0.143f, 0.0f, 50.0f);
	glVertex3f(-0.143f, 0.0f, -50.0f);
	glVertex3f(-0.143f, 0.0f, 50.0f);
	glEnd();
}

void Scene::processPhysics(){
	//for (int objectId = 0; objectId < numOfObjects; ++objectId){
	//	if (objectId > 0){
	//		Ball * ball = (Ball *)(this->objects[objectId]);
	//		ball->collideWithTable();
	//		for (int otherObjectId = objectId + 1; otherObjectId < numOfObjects; ++otherObjectId){
	//			cout << "Collision test: " << objectId << " collides with " << otherObjectId << endl;
	//			Ball * otherBall = (Ball *)(this->objects[otherObjectId]);
	//			ball->collideWithOtherBall(otherBall);
	//		}
	//	}
	//}
}

