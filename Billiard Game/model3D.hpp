#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <map>
#include <vector>
#include <string>

#include "mesh.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "node.hpp"
#include "util.hpp"

using namespace Assimp;

class Model3D{
public:
//private:
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	std::map<std::string, Texture*> textures;
	Node* root;
public:
	Model3D();
	virtual ~Model3D();

	bool loadModel(const std::string& path);
	void clear();

	void render();

	void translate(glm::vec3 distance, const Coordinates& coor, const Coordinates& dis);
	void translate(float dx, float dy, float dz, const Coordinates& coor, const Coordinates& dis);
	void rotate(float angle, glm::vec3 axis, const Coordinates& coor);
	void rotate(float angle, float ox, float oy, float oz, const Coordinates& coor);
	void scale(glm::vec3 factor, const Coordinates& coor);
	void scale(float factor, const Coordinates& coor);
	void scale(float fx, float fy, float fz, const Coordinates& coor);

	glm::mat4 getModelMatrix();
private:
	void importMeshes(const aiScene* scene);
	void importMaterials(const aiScene* scene);
	void importTextures(const aiScene* scene, const std::string& path);
	void importNodes(const aiScene* scene);
	Node* builtNode(const aiNode* node);

	void draw(const Node* node, const glm::mat4& preMatrix);
};
