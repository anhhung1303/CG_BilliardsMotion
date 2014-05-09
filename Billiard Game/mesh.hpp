#pragma once

#include <vector>

#include <gl/glew.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "vertex.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "util.hpp"

#define CLEAR_MESH		0x1
#define CLEAR_MATERIAL	0x2
#define CLEAR_TEXTURE	0x4

class Mesh{
private:
	//Vertex Array Object
	GLuint vao;
	GLuint numIndices;

	const Material* material;
	const Texture* texture;
public:
	static GLuint aPositionLoc;
	static GLuint aTexCoordLoc;
	static GLuint aNormalLoc;

	Mesh();
	virtual ~Mesh();

	void setMesh(const aiMesh* mesh);
	void setMaterial(const Material* material);
	void setTexture(const Texture* texture);

	void clear(GLenum option = CLEAR_MESH | CLEAR_MATERIAL | CLEAR_TEXTURE);

	GLuint getVAO() const;
	GLuint getNumIndices() const;
	const Material* getMaterial() const;
	const Texture* getTexture() const;
};
