#pragma once

#include <map>
#include <vector>
#include <string>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "node.hpp"
#include "util.hpp"

using namespace Assimp;

class Model3D{
public:
	Model3D();
	virtual ~Model3D();

	bool loadModel(const std::string& path);
	void clear();

	void render(const glm::mat4& preMatrix);
private:
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	std::map<std::string, Texture*> textures;
	Node* root;

	void importMeshes(const aiScene* scene);
	void importMaterials(const aiScene* scene);
	void importTextures(const aiScene* scene, const std::string& path);
	void importNodes(const aiScene* scene);
	Node* builtNode(const aiNode* node);

	void draw(Node* node, const glm::mat4& preMatrix);
};
