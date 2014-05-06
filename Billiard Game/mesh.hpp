#pragma once

#include "vertex.hpp"
#include "material.hpp"

#include <gl/glew.h>

#include <vector>

/*
* @breif contain a set of Vertex
*/
class Mesh{
public:
	Mesh();
	Mesh(const std::vector<Vertex>& vertices,
		const std::vector<unsigned int>& indices);
	virtual ~Mesh();
	void clear();

	void setVertices(const std::vector<Vertex>& vertices);
	void setIndices(const std::vector<GLuint>& indices);
	void setMaterial(Material* material);

	GLuint getVertexBufferID() const;
	GLuint getIndexBufferID() const;
	GLuint getNumVertices() const;
	GLuint getNumIndices() const;
	GLuint getVertexBufferData() const;
	GLuint getIndexBufferData() const;
	Material* getMaterial() const;
private:
	GLuint vertexBufferID;
	GLuint indexBufferID;
	GLuint numVertices;
	GLuint numIndices;
	Material* material;
};
