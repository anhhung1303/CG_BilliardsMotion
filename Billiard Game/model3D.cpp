#include "model3D.hpp"

Model3D::Model3D(){
	clear();
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
	for (std::map<std::string, Texture*>::iterator it = textures.begin();
		it != textures.end(); it++){
		delete (it->second);
	}
	meshes.clear();
	materials.clear();
	textures.clear();
	root = NULL;
}

bool Model3D::loadModel(const std::string& path){
	clear();

#ifdef PRINT_LOADING
	fprintf(stdout, "Loading model   : \"%s\" ...\n", path.data());
#endif
	if (!fileIsExist(path)){
		printf("File not fount.\n");
		exit(EXIT_FAILURE);
	}

	static Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(path, aiProcessPreset_TargetRealtime_Quality);
	if (scene == NULL){
		fprintf(stderr, "Error : '%s'\n", imp.GetErrorString());
		exit(EXIT_FAILURE);
	}

#ifdef PRINT_LOADING
	fprintf(stdout, "Loading model DONE.\n");
	fprintf(stdout, "____________________________________________________________\n");
#endif

	importMaterials(scene);
	importTextures(scene, path);
	importMeshes(scene);
	importNodes(scene);

	printf("Mesh     : %d\n", meshes.size());
	printf("Material : %d\n", materials.size());
	printf("Texture  : %d\n", textures.size());
	return true;
}

void Model3D::importMeshes(const aiScene* scene){
	if (!scene->HasMeshes()){
		return;
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; i++){
		Mesh* mes = new Mesh();
		mes->setMesh(scene->mMeshes[i]);

		int matIndex = scene->mMeshes[i]->mMaterialIndex;
		mes->setMaterial(materials[matIndex]);

		aiString file;
		if (scene->mMaterials[matIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == aiReturn_SUCCESS){
			mes->setTexture(textures[file.data]);
		}

		meshes.push_back(mes);
	}
}

void Model3D::importMaterials(const aiScene* scene){
	if (!scene->HasMaterials()){
		return;
	}

	for (unsigned int i = 0; i < scene->mNumMaterials; i++){
		Material* mat = new Material();
		mat->setMaterial(scene->mMaterials[i]);
		materials.push_back(mat);
	}
}

void Model3D::importTextures(const aiScene* scene, const std::string& path){
	aiString file;

	for (unsigned int i = 0; i < scene->mNumMaterials; i++){
		aiMaterial* aiMat = scene->mMaterials[i];

		if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &file) == aiReturn_SUCCESS){
			Texture* tex = new Texture();
			
			if (tex->loadTexture(parsingURL(path, PARSING_DIRECTORY) + "/" +
				parsingURL(file.data, PARSING_DIRECTORY | PARSING_FILE_NAME | PARSING_FILE_EXTENSION))){
				textures[file.data] = tex;
				continue;
			}
			if (tex->loadTexture(parsingURL(path, PARSING_DIRECTORY | PARSING_FILE_NAME) + "/" +
				parsingURL(file.data, PARSING_FILE_NAME | PARSING_FILE_EXTENSION))){
				textures[file.data] = tex;
				continue;
			}
			if (tex->loadTexture(parsingURL(path, PARSING_DIRECTORY) + "/" +
				parsingURL(file.data, PARSING_FILE_NAME | PARSING_FILE_EXTENSION))){
				textures[file.data] = tex;
				continue;
			}
#ifdef PRINT_LOADING
			fprintf(stdout, "CAN'T find any matching texture file.\n");
			fprintf(stdout, "____________________________________________________________\n");
#endif
		}
	}
}

void Model3D::importNodes(const aiScene* scene){
	root = builtNode(scene->mRootNode);
}

Node* Model3D::builtNode(const aiNode* node){
	if (node == NULL){
		return NULL;
	}
	Node* nod = new Node();

	nod->modelMatrix = convertToGLM(node->mTransformation);
	for (unsigned int i = 0; i < node->mNumMeshes; i++){
		int mesIndex = node->mMeshes[i];
		nod->content.push_back(meshes[mesIndex]);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++){
		Node* child = builtNode(node->mChildren[i]);
		nod->children.push_back(child);
	}
	return nod;
}

void Model3D::translate(glm::vec3 distance){
	root->modelMatrix = glm::translate(distance) * root->modelMatrix;
}

void Model3D::rotate(float angle, glm::vec3 axis){
	root->modelMatrix = glm::rotate(angle, axis) * root->modelMatrix;
}

void Model3D::scale(float scaleFactor){
	scale(glm::vec3(scaleFactor));
}

void Model3D::scale(glm::vec3 scaleFactor){
	root->modelMatrix = glm::scale(scaleFactor) * root->modelMatrix;
}

void Model3D::render(){
	draw(root, idMat4);
}

void Model3D::draw(const Node* node, const glm::mat4& preMatrix){
	if (node == NULL){
		return;
	}

	glm::mat4 curMatrix = preMatrix * node->modelMatrix;
	glUniformMatrix4fv(Node::uModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(curMatrix));

	for (const Mesh* mes : node->content){
		const Material* mat = mes->getMaterial();
		const Texture* tex = mes->getTexture();

		if (mat != NULL){
			glBindBufferRange(GL_UNIFORM_BUFFER, Material::uboMaterialLoc,
				mat->getUBOMaterial(), 0, sizeof(LightMaterial));
#ifdef PRINT_LIGHT_MATERIAL
			std::cout << *(mat->uMaterial) << std::endl;
#endif
		}
		if (tex != NULL){
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex->getTextureUnit());
			glUniform1i(Texture::uTextureCountLoc, 1);
		} else {
			glUniform1i(Texture::uTextureCountLoc, NULL);
		}

		glBindVertexArray(mes->getVAO());
		glDrawElements(GL_TRIANGLES, mes->getNumIndices(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(NULL);

		if (tex != NULL){
			glBindTexture(GL_TEXTURE_2D, NULL);
		}
	}

	for (const Node* child : node->children){
		draw(child, curMatrix);
	}
}