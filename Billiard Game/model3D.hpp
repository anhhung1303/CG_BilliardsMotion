#pragma once

#include <map>
#include <vector>

#include <gl/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "mesh.hpp"
#include "material.hpp"

class Model3D{
public:
	Model3D();
	Model3D(const char* path);
	Model3D(const std::string& path);
	virtual ~Model3D();
	void clear();

	void loadModel(const char* path);
	void loadModel(const std::string& path);

	void draw();
private:
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
};

