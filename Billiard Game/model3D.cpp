#include "model3D.hpp"
#include <iostream>

Model3D::Model3D(){
	clear();
}

Model3D::Model3D(const char* path){
	loadModel(path);
}

Model3D::Model3D(const std::string& path){
	loadModel(path.data());
}

Model3D::~Model3D(){
	clear();
}

void Model3D::clear(){
	for (std::vector<Mesh*>::iterator it = meshes.begin();
		it != meshes.end(); it++){
		delete (*it);
	}
	for (std::vector<Material*>::iterator it = materials.begin();
		it != materials.end(); it++){
		delete (*it);
	}
	meshes.clear();
	materials.clear();
}

void Model3D::loadModel(const char* path){
	clear();

	Assimp::Importer imp;
	const aiScene* model = imp.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (model == NULL){
		printf("Error parsing '%s': '%s'\n", path, imp.GetErrorString());
		exit(EXIT_FAILURE);
	}

	//this->meshes.resize(model->mNumMeshes);
	//this->materials.resize(model->mNumMaterials);

	//for (int i = 0; i < model->mNumMeshes; i++){
	//	this->meshes[i] = new Mesh();
	//}
}

void Model3D::loadModel(const std::string& path){
	loadModel(path.data());
}