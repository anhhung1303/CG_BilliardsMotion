#include "mesh.hpp"

GLuint Mesh::aPositionLoc = 0;
GLuint Mesh::aTexCoordLoc = 1;
GLuint Mesh::aNormalLoc = 2;

Mesh::Mesh(){
	clear(CLEAR_MESH | CLEAR_MATERIAL | CLEAR_TEXTURE);
}

Mesh::~Mesh(){
	clear(CLEAR_MESH | CLEAR_MATERIAL | CLEAR_TEXTURE);
}

#include "test_mesh.inl"
void Mesh::setMesh(const aiMesh* mesh){
	clear(CLEAR_MESH);
	GLuint buffer;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	////create array with faces
	////have to convert from Assimp format to array
	//std::vector<unsigned int> indices;
	//for (unsigned int i = 0; i < mesh->mNumFaces; i++){
	//	const aiFace* face = &mesh->mFaces[i];
	//	assert(face->mNumIndices == 3); //If mNumIndices != 3, program rase error
	//	indices.insert(indices.end(), face->mIndices, face->mIndices + sizeof(unsigned int) * (face->mNumIndices - 1));
	//}
	//this->numIndices = indices.size();

	//// buffer for faces
	//glGenBuffers(1, &buffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

	unsigned int *faceArray;
	this->numIndices = mesh->mNumFaces * 3;
	faceArray = new unsigned int[this->numIndices];
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		const aiFace* face = &mesh->mFaces[i];
		memcpy(&faceArray[3 * i], face->mIndices, 3 * sizeof(unsigned int));
	}
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

	// buffer for vertex positions
	if (mesh->HasPositions()) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D)* mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(aPositionLoc);
		glVertexAttribPointer(aPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// buffer for vertex normals
	if (mesh->HasNormals()) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D)* mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(aNormalLoc);
		glVertexAttribPointer(aNormalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// buffer for vertex texture coordinates
	//Now it only support single texture.
	if (mesh->HasTextureCoords(0)) {
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D)* mesh->mNumVertices, mesh->mTextureCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(aTexCoordLoc);
		glVertexAttribPointer(aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(aiVector3D), 0);
	}

	// unbind buffers
	glBindVertexArray(NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}

void Mesh::setMaterial(const Material* material){
	this->material = material;
}

void Mesh::setTexture(const Texture* texture){
	this->texture = texture;
}

void Mesh::clear(GLenum option){
	if ((option & CLEAR_MESH) != 0x0){
		glDeleteVertexArrays(1, &vao);
		numIndices = 0;
	}
	if ((option & CLEAR_MATERIAL) != 0x0){
		material = NULL;
	}
	if ((option & CLEAR_TEXTURE) != 0x0){
		texture = NULL;
	}
}

GLuint Mesh::getVAO() const{
	return this->vao;
}

GLuint Mesh::getNumIndices() const{
	return this->numIndices;
}

const Material* Mesh::getMaterial() const{
	return this->material;
}

const Texture* Mesh::getTexture() const{
	return this->texture;
}