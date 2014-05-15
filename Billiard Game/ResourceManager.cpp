#include "ResourceManager.hpp"

ResourceManager::ResourceManager()
{
	models = NULL;
	programs = NULL;
}


ResourceManager::~ResourceManager()
{
	unload();
}


// function to load resources before drawing
void ResourceManager::load(const char * resourceFilePath)
{
	FILE * inputFile = fopen(resourceFilePath, "r");

	if (inputFile == NULL){
		cerr << "load file " << resourceFilePath << " failed!!!";
	}

	//Reading model
	fscanf(inputFile, "#Models: %d\n", &numOfModels);
	models = new Model3D[numOfModels];
	cout << numOfModels << " ";
	for (int idModel = 0; idModel < numOfModels; ++idModel){
		int temp;
		fscanf(inputFile, "ID %d\n", &temp);
		char fileModel[300];
		fscanf(inputFile, "FILE \"%[^\"]\"\n", fileModel);
		//cout << "File name: " << fileModel << endl;
		models[idModel].loadModel(fileModel);
	}

	//Reading shaders
	fscanf(inputFile, "#Shaders: %d\n", &numOfPrograms);
	programs = new Program[numOfPrograms];
	for (int idShaders = 0; idShaders < numOfPrograms; ++idShaders){
		int temp;
		fscanf(inputFile, "ID %d\n", &temp);
		char fileVertexShader[300], fileFragmentShader[300];
		fscanf(inputFile, "VS \"%[^\"]\"\n", fileVertexShader);
		fscanf(inputFile, "FS \"%[^\"]\"\n", fileFragmentShader);
		cerr << "Shader = " << fileVertexShader << " " << fileFragmentShader;
		programs[idShaders].attachShader(new Shader(fileVertexShader, GL_VERTEX_SHADER));
		programs[idShaders].attachShader(new Shader(fileFragmentShader, GL_FRAGMENT_SHADER));
	}
	fclose(inputFile);
}


// function to unload (clear memory spent for) resources
void ResourceManager::unload()
{
	if (models != NULL){
		delete[] models; 
		models = NULL;
	}
	if (programs != NULL){
		delete[] programs;
		programs = NULL;
	}
}


// get the id model from list models
Model3D * ResourceManager::getModel(int modelId)
{
	return &(models[modelId]);
}

// get the id shader program from list programs
Program * ResourceManager::getProgram(int programId)
{
	return &(programs[programId]);
}